#include "common.h"
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"
#include "pixelfb.h"
#include "trigo.h"
#include "signal.h"
#include "text.h"

#define TEXTURE_WIDTH_MAX 256
#define TEXTURE_X 512
#define TEXTURE_Y 0
#define CLUT_X 512
#define CLUT_Y 256
#define CLUT_WIDTH 256

#define TEXTURE_0_FB_X 512
#define TEXTURE_0_FB_Y 0
#define TEXTURE_0_WIDTH 256

#define IMG_SWAP_VB 60
#define IMG_SWAP_STEPS 16

void intro_msg()
{    
    // upload clut        
    PixelFB clut[CLUT_WIDTH];
    for ( int i = 0; i < CLUT_WIDTH; i++ ) clut[i] = (i & 1) == 1 ? PixelFB::lightgreen() : PixelFB(0, 0, 0, STP_ON);
    LoadClut((u_long*)clut, CLUT_X, CLUT_Y);
    TextureTable tt(TEXTURE_X, TEXTURE_Y);    
    PolyMesg poly_msg (tt, 
        "\n"
        "\n"
        "\n"
        "\n"
        "    This demo is an entry for\n"        
        "PSXDEV Codeblast 2014 competition\n"
        "\n"
        "\n"                
        , 20, 20, 300, 200, -1, CLUT_X, CLUT_Y);                    
    unsigned char b = 0;
    enum { SCENE_SHOW_MESG = 0, SCENE_HIDE_MESG = 1, SCENE_END = 2};
    int current_scene = SCENE_SHOW_MESG;
    int texture_sz_w = SCREEN_WIDTH;
    int texture_sz_h = SCREEN_HEIGHT;
    
    bool display_poly = false;
    POLY_FT4 poly;
    SetPolyFT4(&poly);
    setShadeTex(&poly, SHADE_TEXTURE_OFF);
    setXY4(&poly, 100, 100,
                SCREEN_WIDTH, 100,
                100, SCREEN_HEIGHT,
                SCREEN_WIDTH, SCREEN_HEIGHT);
    setTPage(&poly, TEXTURE_16BIT, BLEND_UNKNOWN, TEXTURE_0_FB_X, TEXTURE_0_FB_Y);
    set_clear_color(0);
    int db = 1;
    while (1)
    {                        
        if ( current_scene == SCENE_SHOW_MESG && b <= 255-db ) {
            b+=db;
            poly_msg.set_brightness(b);                   
            if ( b == 255 ) {
                current_scene++;
            }
        }
        else if ( current_scene == SCENE_HIDE_MESG && b >= db ) {
            b-=db;
            poly_msg.set_brightness(b);                   
            if ( b == 0 ) {                
                current_scene++;
            }
        } else if ( current_scene == SCENE_END ) {
            clear_color_buffer();
            DrawSync(0);
            for ( int i = 0; i < 30; i++ ) VSync(3);
            break;
        }
        clear_color_buffer();
        if ( display_poly == true ) {
            AddPrim(ot, &poly);
        } else {
            poly_msg.draw();    
        }
        display();        
    }   
}