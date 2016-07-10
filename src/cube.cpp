/***
* cude dissolve
*/
#include "common.h"
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>


/**

     6 +------+ 5
      /      /|
  3 /  |  2 / |
   +------+   |
   | 7 +--|---+ 4
   |  /   |  /
   | /    | /
   +------+
  0        1

*/

struct DB {
    POLY_G4 p[6];
    TILE t_bg;
};
static void init_prim(DB* d);

static const int cube_indices[][4] = {
        { 0, 1, 2, 3 },
        { 7, 6, 5, 4 },
        { 1, 4, 5, 2 },
        { 0, 3, 6, 7 },
        { 3, 2, 5, 6 },
        { 0, 7, 4, 1 },
};

static SVECTOR cube_vertices[8] = {
        { -40, -40,  40 },
        {  40, -40,  40 },
        {  40,  40,  40 },
        { -40,  40,  40 },
        {  40, -40, -40 },
        {  40,  40, -40 },
        { -40,  40, -40 },
        { -40, -40, -40 },
};

static const u_char cube_colors[][3] = {
        {  0,    0,    0 },
        {  0xff, 0,    0 },
        {  0xff, 0xff, 0 },
        {  0,    0xff, 0 },
        {  0xff, 0,    0xff },
        {  0xff, 0xff, 0xff },
        {  0,    0xff, 0xff },
        {  0,    0,    0xff },
};

void cube_dissolve()
{   
    DB db[2];
    init_prim(&db[0]);
    init_prim(&db[1]);
    MATRIX m;
    VECTOR trans = {0, 0, 650};
    TransMatrix(&m, &trans);
    SetTransMatrix(&m);   
    int cnt = 0;        
    DB* cdb = &db[0];            
    disable_auto_clear_color_buffer();
    while (cnt != 5 * 10 * 30)
    {        
        SVECTOR rot = {4*cnt%4096, 10*cnt%4096, 0};
        RotMatrix(&rot, &m);
        SetRotMatrix(&m);        
        for ( int i = 0; i < 6; i++ ) {
            long dmy, flag;
            int otz = 0;
            otz += RotTransPers(&cube_vertices[cube_indices[i][0]], (long*)&cdb->p[i].x0, &dmy, &flag);
            otz += RotTransPers(&cube_vertices[cube_indices[i][1]], (long*)&cdb->p[i].x1, &dmy, &flag);
            otz += RotTransPers(&cube_vertices[cube_indices[i][3]], (long*)&cdb->p[i].x2, &dmy, &flag);
            otz += RotTransPers(&cube_vertices[cube_indices[i][2]], (long*)&cdb->p[i].x3, &dmy, &flag);
            otz /= 4;
            limitRange(otz, 0, OT_LENGTH-1);
            AddPrim(ot + OT_LENGTH - 1 - otz, &cdb->p[i]);
        }        
        if ( cnt % 8 == 0) {
            AddPrim(ot, &cdb->t_bg);
        }       
        display();        
        cdb = cdb == &db[0] ? &db[1] : &db[0];
        if ( cnt % 8 == 0) {
            AddPrim(ot, &cdb->t_bg);        
        } cnt++;
    }    
}

static void init_prim(DB* d)
{
    for ( int i = 0; i < 6; i++ ) {
        POLY_G4* p = &d->p[i];
        SetPolyG4(p);                
        const u_char* c;
        c = &cube_colors[cube_indices[i][0]][0]; setRGB0(p, c[0], c[1], c[2]);
        c = &cube_colors[cube_indices[i][1]][0]; setRGB1(p, c[0], c[1], c[2]);
        c = &cube_colors[cube_indices[i][3]][0]; setRGB2(p, c[0], c[1], c[2]);
        c = &cube_colors[cube_indices[i][2]][0]; setRGB3(p, c[0], c[1], c[2]);
        
        TILE* t_bg = &d->t_bg;        
        SetTile(t_bg);
        SetSemiTrans(t_bg, 1);
        setXY0(t_bg, 0, 0);
        setWH(t_bg, 320, 240);
        setRGB0(t_bg, 0, 0, 0);
    }
}
