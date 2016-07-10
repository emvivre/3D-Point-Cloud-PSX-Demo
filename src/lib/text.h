#ifndef _TEXT_H_
#define _TEXT_H_

#include "font.h"

struct POLY_FT4;

// generate a texture of font then upload it VRAM 
// 95 characters of 8 / 2 pixels in VRAM
// 95 * 8 / 2 -> 380 pixels FB
//     1 line of 256 pixels 
//     1 line of 124 pixels
class TextureTable
{   
    int texture_x, texture_y;       
    const int nb_character_per_line;
public:    
    TextureTable(int texture_x, int texture_y);
    inline int x() const { return texture_x; }
    inline int y() const { return texture_y; }
    struct UV { 
        int u, v; 
        UV(int u, int v) : u(u), v(v) {}
    };
    inline UV operator[](char c) const {
        int u = ((c - FONT_BEGIN) % nb_character_per_line) * FONT_WIDTH;
        int v = ((c - FONT_BEGIN) / nb_character_per_line) * FONT_HEIGHT;
        return UV(u, v);
    }
};

class PolyMesg
{
    POLY_FT4* poly;
    int nb_poly;
    char* poly_txt;
    int nb_line_tot;
    int current_line;
    int nb_line_displayed;
    int nb_poly_per_line;
    const TextureTable* ttt;   
    int y_shift;
    int x_beg;
    int poly_w;
    unsigned int y_shift_idx;
    int y_beg;
    int y_end;
public:    
    PolyMesg(const TextureTable& tt, const char* msg, int x_beg, int y_beg, int w, int h, int nb_line_displayed, int clut_x, int clut_y, char font_begin = '0');
    ~PolyMesg();
    void scroll_down();
    void set_brightness(unsigned char b);
    void draw();
    inline int get_nb_line_tot() const { return nb_line_tot; }
    inline int get_current_line() const { return current_line; }
    inline int get_nb_line_displayed () const { return nb_line_displayed; }    
};

#endif