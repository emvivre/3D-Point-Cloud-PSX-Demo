#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>
#include <stdlib.h>
#include <string.h>
#include "pixelfb.h"
#include "trigo.h"
#include "signal.h"
#include "credit_part0.h"
#include "credit_part1.h"
#include "util.h"
#include "text.h"
#include "common.h"

#define TEXTURE_WIDTH_MAX 256
#define TEXTURE_X 512
#define TEXTURE_Y 0
#define CLUT_X 512
#define CLUT_Y 256
#define CLUT_WIDTH 256

#define TEXTURE_0_FB_X 768
#define TEXTURE_0_FB_Y 0
#define TEXTURE_0_WIDTH 160
#define TEXTURE_0_HEIGHT 240

#define TEXTURE_1_FB_X TEXTURE_0_FB_X
#define TEXTURE_1_FB_Y (TEXTURE_0_FB_Y+256)
#define TEXTURE_1_WIDTH TEXTURE_0_WIDTH
#define TEXTURE_1_HEIGHT TEXTURE_0_HEIGHT

// image 320x240
class ImageBackground
{
    POLY_FT4 p_left;
    POLY_FT4 p_right;
public:
    ImageBackground(const unsigned char* texture0, int texture_0_fb_x, int texture_0_fb_y, int texture_0_width, int texture_0_height, const unsigned char* texture1, int texture_1_fb_x, int texture_1_fb_y, int texture_1_width, int texture_1_height) {
        SetPolyFT4(&p_left);
        setXYWH(&p_left, 0, 0, texture_0_width, texture_0_height);
        SetShadeTex(&p_left, STP_ON);    
        setUVWH(&p_left, 0, 0, texture_0_width, texture_0_height);
        setTPage(&p_left, TEXTURE_16BIT, BLEND_UNKNOWN, texture_0_fb_x, texture_0_fb_y);       
        
        SetPolyFT4(&p_right);    
        setXYWH(&p_right, texture_0_width, 0, texture_1_width, texture_1_height);
        SetShadeTex(&p_right, STP_ON);
        setUVWH(&p_right, 0, 0, texture_1_width, texture_1_height);
        setTPage(&p_right, TEXTURE_16BIT, BLEND_UNKNOWN, texture_1_fb_x, texture_1_fb_y);

        RECT texture_pos0 = { texture_0_fb_x, texture_0_fb_y, texture_0_width, texture_0_height };
        LoadImage(&texture_pos0, (u_long*)credit_part0_texture);
        RECT texture_pos1 = { texture_1_fb_x, texture_1_fb_y, texture_1_width, texture_1_height };
        LoadImage(&texture_pos1, (u_long*)credit_part1_texture);
        
        set_brightness(255);
    }    
    void set_brightness(unsigned char b) {
        setRGB0(&p_left, b, b, b);
        setRGB0(&p_right, b, b, b);
    }
    void draw() {
        AddPrim(ot, &p_left);
        AddPrim(ot, &p_right);
    }
};

void credits()
{   
    int t = 0;    
    PixelFB clut[CLUT_WIDTH];
    for ( int i = 0; i < CLUT_WIDTH; i++ ) clut[i] = (i & 1) == 1 ? PixelFB::lightgreen() : PixelFB(0, 0, 0, STP_ON);
    LoadClut((u_long*)clut, CLUT_X, CLUT_Y);    
    TextureTable tt(TEXTURE_X, TEXTURE_Y);
    const int NB_LINE_DISPLAYED = 15;
    PolyMesg poly_msg (tt,    
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        #if 1
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        #endif
        "\n"                
        "          You end the demo!\n"
        "\n"
        "The 3D point cloud contains exacly \n"
        "40k vertices! This latter was\n"
        "obtained using a real 3D scanner.\n"
        "All assets of demo except the XA\n"
        "music are included in the psx-exe!\n"
        "Yes, the point cloud is included in\n"
        "the psx-exe too. Without any\n"
        "specifical optimization, bigger\n"
        "point cloud could be shown.\n"
        "\n"
        "This demo was really short. Indeed,\n"
        "I discovered the sdk very late (at\n"
        "the end of april) then I started to\n"
        "develop tools and small apps to test\n"
        "the little boy machine :D Source code\n"
        "of this demo is provided with this\n"
        "demo. Of course, the demo does not\n"
        "use any \"buggy\" implementation of\n"
        "malloc[23]() that comes from Sony.\n"       
        "\n"
        "I hope learn more about the psx,\n"
        "and doing funny projects like reverse\n"
        "engineering, game enhancement, ... \n"
        "So if you are interesting in doing\n"
        "game translation, or any amazing\n"
        "project involving psx please pm me.\n"
        "\n"
        "   Thanks to psxdev community, and\n"
        "all psxdev members, for keeping \n"
        "interesting information about\n"
        "the psx, the forum, tutorials,...\n"
        "\n"
        "You can join us in the psxdev irc\n"
        "channel on the\n"
        " EFNET server(irc.eversible.com/6667)\n"       
        " channel: #psxdev\n"
        "\n"
        "@Stamba: merci de m'avoir transmis\n"
        "         la musique dans les temps :D\n"
        "\n"        
        "                     !* S.T.U-Emvivre\n"
        "\n"
        "\n"
        "*************************************\n"
        "*             CREDITS               *\n"
        "*                                   *\n"
        "*     Coder: !* S.T.U-Emvivre       *\n"
        "*       GFX: !* S.T.U-Vash          *\n"
        "*   Music 0: Stamba                 *\n"
        "*   Music 1: !* S.T.U-Vash          *\n"
        "*************************************\n"       
        , 10, 10, 320, 240, NB_LINE_DISPLAYED, CLUT_X, CLUT_Y);         
    ImageBackground bg(
        (const unsigned char*)credit_part0_texture, TEXTURE_0_FB_X, TEXTURE_0_FB_Y, TEXTURE_0_WIDTH, TEXTURE_0_HEIGHT, 
        (const unsigned char*)credit_part1_texture, TEXTURE_1_FB_X, TEXTURE_1_FB_Y, TEXTURE_1_WIDTH, TEXTURE_1_HEIGHT);
    const int BRIGHTNESS_MIN = 50;
    const int BRIGHTNESS_MAX = 255;
    const int BRIGHTNESS_NB_STEP = 20;
    const int BRIGHTNESS_LINE_OFFSET = 4;
    bg.set_brightness(50);      
    SignalProcessing::LinearSignal ls(BRIGHTNESS_MIN, BRIGHTNESS_MAX, BRIGHTNESS_NB_STEP);    
    ls.set_value(BRIGHTNESS_MAX);
    ls.down();
    const int bg_nb_line =  poly_msg.get_nb_line_tot();
    set_clear_color(0);        
    enable_auto_clear_color_buffer();    
    while (1)
    {                        
        int bg_cl = poly_msg.get_current_line() % bg_nb_line;        
        if ( bg_cl > bg_nb_line - BRIGHTNESS_LINE_OFFSET || bg_cl < BRIGHTNESS_LINE_OFFSET ) {
            ls.up();
        }
        else {
            ls.down();
        }       
        ls.update();
        if ( t % 3 == 0 ) 
            poly_msg.scroll_down();                  
        bg.set_brightness(ls.value());
        bg.draw();
        poly_msg.draw();            
        display();        
        t++;
    }    
}