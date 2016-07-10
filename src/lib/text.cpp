#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include "text.h"
#include "trigo.h"
#include "signal.h"
#include "pixelfb.h"
#include "mm.h"
#include <string.h>

#define TEXTURE_WIDTH_MAX 256

extern u_long* ot;
extern const int OT_LENGTH;

TextureTable::TextureTable(int texture_x, int texture_y) : texture_x(texture_x), texture_y(texture_y), nb_character_per_line(TEXTURE_WIDTH_MAX/FONT_WIDTH-1) {  // -1 : avoid sony last column bug !
    int nb_character = FONT_END - FONT_BEGIN + 1;        
    const int texture_width = nb_character_per_line * FONT_WIDTH;
    int nb_lines = ((nb_character-1) / nb_character_per_line+1);
    const int texture_height = nb_lines * FONT_HEIGHT;
    int sz = texture_width * texture_height / 2;
    unsigned char* texture = (unsigned char*)MemoryManager::malloc(sz);
    memset(texture, 0, sz);                
    for (int i = 0; i < nb_character; i++ ) {
        int line_i = i / nb_character_per_line;
        unsigned char* pattern = &font_ascii[i][0];
        for ( int y = 0; y < FONT_HEIGHT; y++ ) {
            for (int x = 0; x < FONT_WIDTH; x++ ) {
                unsigned char b = ((pattern[y] >> x) & 1);
                unsigned char v = b << (4 * (x&1));
                texture[line_i * texture_width * (FONT_HEIGHT-1) / 2 + y * texture_width/2 + i*FONT_WIDTH/2 + x/2] |=  v;
            }
        }
    }        
    RECT r;
    setRECT(&r, texture_x, texture_y, texture_width/4, texture_height);
    LoadImage(&r, (u_long*)texture);
    DrawSync(0);
    MemoryManager::free(texture);
}

#define UV_WIDTH 4*FONT_WIDTH

// nb_line_displayed : if < 0 -> keep ratio (w,h) character : abs(v) -> zoom
PolyMesg::PolyMesg(const TextureTable& tt, const char* msg, int x_beg, int y_beg, int w, int h, int nb_line_displayed, int clut_x, int clut_y, char font_begin = '0') : nb_line_displayed(nb_line_displayed), y_shift(0), x_beg(x_beg), y_shift_idx(0), y_beg(y_beg), y_end(y_beg+h) {        
    this->ttt = &tt;
    this->current_line = 0;
    this->nb_line_tot = 0;
    int nb_char_per_line = 0;
    int nb_char_per_line_max = 0;                
    const char* p = msg;
    while(true) {                                    
        if (*p == '\n' || *p == '\0') { 
            this->nb_line_tot++;
            if ( nb_char_per_line > nb_char_per_line_max ) nb_char_per_line_max = nb_char_per_line;
            nb_char_per_line = 0;
            if (*p == '\0') break;
        } else {
            nb_char_per_line++;
        }     
        p++;            
    }                
    if ( this->nb_line_displayed < 0 ) {
        int zoom = -this->nb_line_displayed;
        this->nb_poly_per_line = w / FONT_WIDTH;
        this->nb_line_displayed = zoom * h / FONT_HEIGHT; 
    } else {
        this->nb_poly_per_line = nb_char_per_line_max;
    }        
    this->nb_poly = this->nb_poly_per_line * (this->nb_line_displayed+1);
    this->poly = (POLY_FT4*) MemoryManager::malloc(sizeof(POLY_FT4) * this->nb_poly);
    int poly_txt_sz = this->nb_line_tot * this->nb_poly_per_line;
    this->poly_txt = (char*) MemoryManager::malloc(poly_txt_sz);
    p = msg;          
    for ( int y = 0; y < this->nb_line_tot; y++ ) {
        int x;
        for ( x = 0; x < this->nb_poly_per_line && (*p != '\n' && *p != '\0'); x++, p++) {
            poly_txt[y * this->nb_poly_per_line + x] = *p;
        }
        for ( ; x < this->nb_poly_per_line; x++ ) {
           poly_txt[y * this->nb_poly_per_line + x] = ' ';
        }
        if ( *p != '\0' ) p++;
    }
    POLY_FT4* pft4 = this->poly;
    poly_w = w / this->nb_poly_per_line;
    int poly_h = h / this->nb_line_displayed;        
    for ( int y = 0; y < this->nb_line_displayed+1; y++ ) {
        for ( int x = 0; x < this->nb_poly_per_line; x++, pft4++ ) { 
            SetPolyFT4(pft4);
            SetShadeTex(pft4, SHADE_TEXTURE_ON);
            setXYWH(pft4, x_beg + x * poly_w, y_beg + y * poly_h, poly_w, poly_h);
            setTPage(pft4, TEXTURE_4BIT, BLEND_UNKNOWN, tt.x(), tt.y());
            setClut(pft4, clut_x, clut_y);                
            char c = poly_txt[(y % this->nb_line_tot) * this->nb_poly_per_line + x];                
            TextureTable::UV uv = (*this->ttt)[c];
            setUVWH(pft4, uv.u, uv.v, FONT_WIDTH, FONT_HEIGHT);           
        }
    }
}

PolyMesg::~PolyMesg() {
    MemoryManager::free(this->poly);
    MemoryManager::free(this->poly_txt);
}

#define LINE_SHADED 40
void PolyMesg::scroll_down() {        
    int dy = poly[0].y0 == -(FONT_HEIGHT-1) ? FONT_HEIGHT-1 : -1; 
    POLY_FT4* pft4 = poly;         
    y_shift_idx++;
    for ( int y = 0; y < this->nb_line_displayed+1; y++ ) {
        unsigned char b = 0;
        int y_beg_dist = pft4->y0 - y_beg; if ( y_beg_dist < 0 ) y_beg_dist = 0;
        int y_end_dist = y_end - pft4->y3; if ( y_end_dist < 0 ) y_end_dist = 0;
        if ( y_beg_dist < LINE_SHADED ) b = (y_beg_dist * 255) / LINE_SHADED;
        else if ( y_end_dist < LINE_SHADED ) b = y_end_dist * 255 / LINE_SHADED;
        else b = 255;            
        int offx = sin4k[4096/2 + SignalProcessing::unitary_to_triangle_signal(y + y_shift_idx, 40)]/10;
        for ( int x = 0; x < this->nb_poly_per_line; x++, pft4++ ) {        
            setRGB0(pft4, b, b, b);
            pft4->y0 += dy;
            pft4->y1 += dy;
            pft4->y2 += dy;
            pft4->y3 += dy;
            pft4->x0 = pft4->x2 = x_beg + x * poly_w + offx;
            pft4->x1 = pft4->x3 = pft4->x0 + poly_w;                
        }
    }
    if ( dy != -1 ) {            
        current_line++;            
        POLY_FT4* pft4 = poly;
        for ( int y = 0; y < this->nb_line_displayed+1; y++ ) {
            for ( int x = 0; x < this->nb_poly_per_line; x++, pft4++ ) {            
                char c = poly_txt[((current_line + y) % this->nb_line_tot) * this->nb_poly_per_line + x];
                TextureTable::UV uv = (*this->ttt)[c];
                setUVWH(pft4, uv.u, uv.v, FONT_WIDTH, FONT_HEIGHT);
            }                
        }            
    }
}

void PolyMesg::draw() {    
    for ( int i = 0; i < nb_poly; i++ ) {
        AddPrim(ot+OT_LENGTH-1, &poly[i]);
    }
}

void PolyMesg::set_brightness(unsigned char b) {
    POLY_FT4* p = poly;
    for ( int i = 0; i < nb_poly; i++, p++ ) {
        SetShadeTex(p, SHADE_TEXTURE_ON);            
        setRGB0(p, b, b, b);
    }    
}
