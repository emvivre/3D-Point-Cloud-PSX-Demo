#include "common.h"
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>
#include "mm.h"
#include "pixelfb.h"
#include "trigo.h"
#include "signal.h"
#include "util.h"
#include <stdio.h>

void sphere()
{    
    SetGeomScreen(1000);     
    MATRIX m;
    VECTOR trans = {0, 0, 300};
    TransMatrix(&m, &trans);
    SetTransMatrix(&m);      
    const int SPHERE_LATITUDE = 24;
    const int SPHERE_LONGITUDE = 24;
    const int SPHERE_RADIUS = 50;
    SVECTOR* sphere_vector = (SVECTOR*)MemoryManager::malloc(SPHERE_LATITUDE * SPHERE_LONGITUDE * sizeof(*sphere_vector));   
    POLY_G4* poly_sphere = (POLY_G4*)MemoryManager::malloc(SPHERE_LATITUDE * SPHERE_LONGITUDE * sizeof(*poly_sphere));
    {
        /**
         *  x <- r * sin(\alpha) * cos(\beta)
         *  y <- r * sin(\alpha) * sin(\beta)
         *  z <- r * cos(\alpha)
         * avec:
         *   \alpha \in [0, pi] : latitude
         *   \beta \in [0, 2*pi] : longitude
         */ 
        SVECTOR* v = sphere_vector;
        for ( int la = 0; la < SPHERE_LATITUDE; la++ ) {
            int alpha = (TRIGO_PI * la) / SPHERE_LATITUDE;
            int sa = sin4k[alpha];
            int ca = cos4k[alpha];
            for ( int lo = 0; lo < SPHERE_LONGITUDE; lo++, v++ ) {
                int beta = (2 * TRIGO_PI * lo) / SPHERE_LONGITUDE;
                unsigned short x = (SPHERE_RADIUS * sa * cos4k[beta]) / (TRIGO_SCALE * TRIGO_SCALE);
                unsigned short y = (SPHERE_RADIUS * sa * sin4k[beta]) / (TRIGO_SCALE *TRIGO_SCALE);
                unsigned short z = (SPHERE_RADIUS * ca) / TRIGO_SCALE;
                setVector(v, x, y, z);             
            }            
        }
        POLY_G4* p = poly_sphere;        
        for ( int la = 0; la < SPHERE_LATITUDE; la++ ) {
            for ( int lo = 0; lo < SPHERE_LONGITUDE; lo++, p++ ) {
                SetPolyG4(p);
            }
        }        
    }
    set_clear_color(0);
    static const int SPHERE_TRANSITION_STEP = 200;
    static const int SPHERE_STEP_TIME = 600;
            
    enum { SCENE_LAT_FLAT = 1, 
           SCENE_LAT_SMOOTH,
           SCENE_LIGHT_FIX_EDGE,
           SCENE_LIGHT_FIX_SQUARE,
           SCENE_LIGHT_FIX,
           SCENE_LIGHT_MOVE,
           SCENE_LIGHT_CUTOFF,
           SCENE_ZOOM,
           SCENE_LIGHT_SINUS_WAVE, };
    int current_scene = SCENE_LAT_FLAT;
    int t = 0;
    enable_auto_clear_color_buffer();    
    while ( t < SCENE_LIGHT_SINUS_WAVE * SPHERE_STEP_TIME)
    {
        SVECTOR rot = {8*t%4096, 4*t%4096, 10*t%4096};
        RotMatrix(&rot, &m);
        SetRotMatrix(&m);
        
        // update sphere    
        POLY_G4* p = poly_sphere;        
        
        // offset plasma
        static int off = 0; 
        if ( t >= 100 && t % 5 == 0 ) off++;  
                
        // light move
        static int light_move_lat = 0;
        static int light_move_long = 0;
        static int light_move_dlat = 1;
        static int light_move_dlong = 1;           
        if ( t % 1 == 0 ) {
            if ( t % 3 == 0 ) {
                light_move_lat += light_move_dlat; 
                if ( light_move_lat < 0 ) light_move_lat = 0; 
                if ( light_move_lat >= SPHERE_LATITUDE ) light_move_lat = SPHERE_LATITUDE-1;
                if ( light_move_lat == 0 || light_move_lat == SPHERE_LATITUDE-1 ) light_move_dlat *= -1;
            }
            if ( t % 1 == 0 ) {
                light_move_long += light_move_dlong; 
                if ( light_move_long < 0 ) light_move_long = 0; 
                if ( light_move_long >= SPHERE_LONGITUDE ) light_move_long = SPHERE_LONGITUDE-1;
                if ( light_move_long == 0 || light_move_long == SPHERE_LONGITUDE-1 ) light_move_long *= -1;
            }
        }     

        // dynamic cutoff
        static int light_dynamic_distance_max = 50;        
        if ( current_scene == SCENE_LIGHT_CUTOFF && t % 1 == 0 ) {            
            static const int LIGHT_DYNAMIC_DISTANCE_MIN = 20;
            static const int LIGHT_DYNAMIC_DISTANCE_MAX = 60;
            static int light_dynamic_ddist = -1;
            light_dynamic_distance_max += light_dynamic_ddist;
            if ( light_dynamic_distance_max <= LIGHT_DYNAMIC_DISTANCE_MIN || light_dynamic_distance_max >= LIGHT_DYNAMIC_DISTANCE_MAX ) light_dynamic_ddist *= -1;
        }
        int light_dynamic_distance_max2 = light_dynamic_distance_max * light_dynamic_distance_max;   
        
        // dynamic radius         
        if ( current_scene >= SCENE_ZOOM && t % 1 == 0 ) {                        
            static const int ZOOM_MIN = 150;
            static const int ZOOM_MAX = 900;
            static int zoom_stride = 15;
            static int zoom = 300;            
            SetGeomScreen(zoom);            
            zoom += zoom_stride;
            if ( zoom <= ZOOM_MIN || zoom >= ZOOM_MAX ) zoom_stride *= -1;
        }        
                
        int dt = 1;
        for ( int la = 0; la < SPHERE_LATITUDE; la++ ) {
            for ( int lo = 0; lo < SPHERE_LONGITUDE; lo++, p++ ) {                
                int lo_next = (lo+1)%SPHERE_LONGITUDE;
                int la_next = SignalProcessing::min(la+1, SPHERE_LATITUDE-1);                
                SVECTOR* v0 = &sphere_vector[la*SPHERE_LONGITUDE+lo];
                SVECTOR* v1 = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                SVECTOR* v2 = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];
                SVECTOR* v3 = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                
                long dmy, flag;
                int otz = 0;
                otz += RotTransPers(v0, (long*)&p->x0, &dmy, &flag);                 
                otz += RotTransPers(v1, (long*)&p->x1, &dmy, &flag);                 
                otz += RotTransPers(v2, (long*)&p->x2, &dmy, &flag);                
                otz += RotTransPers(v3, (long*)&p->x3, &dmy, &flag);
                otz /= 4;
                limitRange(otz, 0, OT_LENGTH-1);
                AddPrim(ot + OT_LENGTH - 1 - otz, p);                
                if ( t < SCENE_LAT_FLAT*SPHERE_STEP_TIME ) { // latitude same flat color
                    current_scene = SCENE_LAT_FLAT;
                    unsigned char b = 255 * la / SPHERE_LATITUDE;                     
                    setRGB0(p, b, b, b);
                    setRGB1(p, b, b, b);
                    setRGB2(p, b, b, b);
                    setRGB3(p, b, b, b);
                } else if ( t < SCENE_LAT_SMOOTH*SPHERE_STEP_TIME ) { // latitude same smooth color
                    current_scene = SCENE_LAT_SMOOTH;
                    unsigned char b = 255 * la / SPHERE_LATITUDE;  
                    unsigned char bb = 255 * ((la+1)%SPHERE_LATITUDE) / SPHERE_LATITUDE;
                    if ( t < (SCENE_LAT_SMOOTH-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB1(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB3(p, bb, bb, bb);
                } else if ( t < SCENE_LIGHT_FIX_EDGE*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting fix
                    dt = 3;
                    current_scene = SCENE_LIGHT_FIX_EDGE;
                    unsigned char b = 255;
                    unsigned char bb = 255;
                    const int LIGHT_LAT = 3*SPHERE_LATITUDE/4;
                    const int LIGHT_LONG = SPHERE_LONGITUDE/4;
                    const int LIGHT_DISTANCE_MAX = 50;
                    const int LIGHT_DISTANCE_MAX2 = LIGHT_DISTANCE_MAX * LIGHT_DISTANCE_MAX;
                    SVECTOR* l = &sphere_vector[LIGHT_LAT*SPHERE_LONGITUDE+LIGHT_LONG];
                    SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];
                    SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];
                    int d = SignalProcessing::dist2(l, v);
                    int dd = SignalProcessing::dist2(l, vv);
                    b = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - d) * b) / LIGHT_DISTANCE_MAX2;
                    bb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dd) * bb) / LIGHT_DISTANCE_MAX2;
                    // smooth
                    if ( t < (SCENE_LIGHT_FIX-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB1(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB3(p, bb, bb, bb);
                } else if ( t < SCENE_LIGHT_FIX_SQUARE*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting fix
                    dt = 3;
                    current_scene = SCENE_LIGHT_FIX_SQUARE;
                    unsigned char b = 255;
                    unsigned char bb = 255;
                    unsigned char bbb = 255;
                    unsigned char bbbb = 255;
                    const int LIGHT_LAT = 3*SPHERE_LATITUDE/4;
                    const int LIGHT_LONG = SPHERE_LONGITUDE/4;
                    const int LIGHT_DISTANCE_MAX = 50;
                    const int LIGHT_DISTANCE_MAX2 = LIGHT_DISTANCE_MAX * LIGHT_DISTANCE_MAX;
                    SVECTOR* l = &sphere_vector[LIGHT_LAT*SPHERE_LONGITUDE+LIGHT_LONG];
                    SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                    SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                    SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                    SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                    int d = SignalProcessing::dist2(l, v);
                    int dd = SignalProcessing::dist2(l, vv);
                    int ddd = SignalProcessing::dist2(l, vvv);
                    int dddd = SignalProcessing::dist2(l, vvvv);
                    b = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - d) * b) / LIGHT_DISTANCE_MAX2;
                    bb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dd) * bb) / LIGHT_DISTANCE_MAX2;
                    bbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - ddd) * bbb) / LIGHT_DISTANCE_MAX2;
                    bbbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - dddd) * bbbb) / LIGHT_DISTANCE_MAX2;                    
                    // smooth
                    if ( t < (SCENE_LIGHT_FIX-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB1(p, bb, bb, bb);
                    setRGB2(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);                     
                } else if ( t < SCENE_LIGHT_FIX*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting fix
                    dt = 1;
                    current_scene = SCENE_LIGHT_FIX;
                    unsigned char b = 255;
                    unsigned char bb = 255;
                    unsigned char bbb = 255;
                    unsigned char bbbb = 255;
                    const int LIGHT_LAT = 3*SPHERE_LATITUDE/4;
                    const int LIGHT_LONG = SPHERE_LONGITUDE/4;
                    const int LIGHT_DISTANCE_MAX = 50;
                    const int LIGHT_DISTANCE_MAX2 = LIGHT_DISTANCE_MAX * LIGHT_DISTANCE_MAX;
                    SVECTOR* l = &sphere_vector[LIGHT_LAT*SPHERE_LONGITUDE+LIGHT_LONG];
                    SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                    SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                    SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                    SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                    int d = SignalProcessing::dist2(l, v);
                    int dd = SignalProcessing::dist2(l, vv);
                    int ddd = SignalProcessing::dist2(l, vvv);
                    int dddd = SignalProcessing::dist2(l, vvvv);
                    b = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - d) * b) / LIGHT_DISTANCE_MAX2;
                    bb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dd) * bb) / LIGHT_DISTANCE_MAX2;
                    bbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - ddd) * bbb) / LIGHT_DISTANCE_MAX2;
                    bbbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - dddd) * bbbb) / LIGHT_DISTANCE_MAX2;                    
                    // smooth
                    if ( t < (SCENE_LIGHT_FIX-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r1 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r3 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB1(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);                    
                } else if ( t < SCENE_LIGHT_MOVE*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting move
                    current_scene = SCENE_LIGHT_MOVE;
                    unsigned char b = 255;
                    unsigned char bb = 255;                    
                    unsigned char bbb = 255;                    
                    unsigned char bbbb = 255;                    
                    const int LIGHT_DISTANCE_MAX = 50;
                    const int LIGHT_DISTANCE_MAX2 = LIGHT_DISTANCE_MAX * LIGHT_DISTANCE_MAX;
                    // light
                    {
                        SVECTOR* l = &sphere_vector[light_move_lat*SPHERE_LONGITUDE+light_move_long];
                        SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                        SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                        int d = SignalProcessing::dist2(l, v);
                        int dd = SignalProcessing::dist2(l, vv);
                        int ddd = SignalProcessing::dist2(l, vvv);
                        int dddd = SignalProcessing::dist2(l, vvvv);
                        b = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - d) * b) / LIGHT_DISTANCE_MAX2;
                        bb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dd) * bb) / LIGHT_DISTANCE_MAX2;
                        bbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - ddd) * bbb) / LIGHT_DISTANCE_MAX2;
                        bbbb = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - dddd) * bbbb) / LIGHT_DISTANCE_MAX2;                       
                    }
                    // smooth
                    if ( t < (SCENE_LIGHT_MOVE-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r1 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r3 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB1(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);      
                } else if ( t < SCENE_LIGHT_CUTOFF*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting move + dynamic cutoff 
                    current_scene = SCENE_LIGHT_CUTOFF;
                    unsigned char b = 255;
                    unsigned char bb = 255;
                    unsigned char bbb = 255;
                    unsigned char bbbb = 255;
                    // light
                    {
                        SVECTOR* l = &sphere_vector[light_move_lat*SPHERE_LONGITUDE+light_move_long];
                        SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                        SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                        int d = SignalProcessing::dist2(l, v);
                        int dd = SignalProcessing::dist2(l, vv);
                        int ddd = SignalProcessing::dist2(l, vvv);
                        int dddd = SignalProcessing::dist2(l, vvvv);
                        b = (SignalProcessing::max(0, light_dynamic_distance_max2 - d) * b) / light_dynamic_distance_max2;
                        bb = (SignalProcessing::max(0,  light_dynamic_distance_max2 - dd) * bb) / light_dynamic_distance_max2;                    
                        bbb = (SignalProcessing::max(0, light_dynamic_distance_max2 - ddd) * bbb) / light_dynamic_distance_max2;
                        bbbb = (SignalProcessing::max(0, light_dynamic_distance_max2 - dddd) * bbbb) / light_dynamic_distance_max2;
                    }
                    // smooth
                    if ( t < (SCENE_LIGHT_CUTOFF-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r1 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r3 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB1(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);     
                } else if ( t < SCENE_ZOOM*SPHERE_STEP_TIME ) { // latitude same smooth color + lighting move + dynamic cutoff + zoom
                    current_scene = SCENE_ZOOM;
                    unsigned char b = 255;
                    unsigned char bb = 255;
                    unsigned char bbb = 255;
                    unsigned char bbbb = 255;
                    // light
                    {
                        SVECTOR* l = &sphere_vector[light_move_lat*SPHERE_LONGITUDE+light_move_long];
                        SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                        SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                        int d = SignalProcessing::dist2(l, v);
                        int dd = SignalProcessing::dist2(l, vv);
                        int ddd = SignalProcessing::dist2(l, vvv);
                        int dddd = SignalProcessing::dist2(l, vvvv);
                        b = (SignalProcessing::max(0, light_dynamic_distance_max2 - d) * b) / light_dynamic_distance_max2;
                        bb = (SignalProcessing::max(0,  light_dynamic_distance_max2 - dd) * bb) / light_dynamic_distance_max2;                    
                        bbb = (SignalProcessing::max(0, light_dynamic_distance_max2 - ddd) * bbb) / light_dynamic_distance_max2;
                        bbbb = (SignalProcessing::max(0, light_dynamic_distance_max2 - dddd) * bbbb) / light_dynamic_distance_max2;                   
                    }
                    // smooth
                    if ( t < (SCENE_ZOOM-1)*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r1 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r3 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB1(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);
                }                 
                else { // smooth bande move + lighting move
                    current_scene = SCENE_LIGHT_SINUS_WAVE;
                    const int LIGHT_DISTANCE_MAX = 50;
                    const int LIGHT_DISTANCE_MAX2 = LIGHT_DISTANCE_MAX * LIGHT_DISTANCE_MAX;
                    const int NB_COLOR = 5;                    
                    unsigned char b = (255 * (((la+off)%SPHERE_LATITUDE)%NB_COLOR)) / (NB_COLOR-1);
                    unsigned char bb = (255 * (((la+1+off)%SPHERE_LATITUDE)%NB_COLOR)) / (NB_COLOR-1);
                    unsigned char bbb = (255 * (((la+off)%SPHERE_LATITUDE)%NB_COLOR)) / (NB_COLOR-1);
                    unsigned char bbbb = (255 * (((la+1+off)%SPHERE_LATITUDE)%NB_COLOR)) / (NB_COLOR-1);
                    // light
                    {
                        SVECTOR* l = &sphere_vector[light_move_lat*SPHERE_LONGITUDE+light_move_long];
                        SVECTOR* v = &sphere_vector[la*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo];                    
                        SVECTOR* vvv = &sphere_vector[la*SPHERE_LONGITUDE+lo_next];
                        SVECTOR* vvvv = &sphere_vector[la_next*SPHERE_LONGITUDE+lo_next];
                        int d = SignalProcessing::dist2(l, v);
                        int dd = SignalProcessing::dist2(l, vv);
                        int ddd = SignalProcessing::dist2(l, vvv);
                        int dddd = SignalProcessing::dist2(l, vvvv);
                        b = (SignalProcessing::max(0, LIGHT_DISTANCE_MAX2 - d) * b) / LIGHT_DISTANCE_MAX2;
                        bb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dd) * bb) / LIGHT_DISTANCE_MAX2; 
                        bbb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - ddd) * bbb) / LIGHT_DISTANCE_MAX2; 
                        bbbb = (SignalProcessing::max(0,  LIGHT_DISTANCE_MAX2 - dddd) * bbbb) / LIGHT_DISTANCE_MAX2; 
                    }
                    // smooth
                    if ( t < 5*SPHERE_STEP_TIME + SPHERE_TRANSITION_STEP ) {
                        int d= (t%SPHERE_TRANSITION_STEP);
                        b = (b * d + p->r0 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bb = (bb * d + p->r2 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbb = (bbb * d + p->r1 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                        bbbb = (bbbb * d + p->r3 * (SPHERE_TRANSITION_STEP - d)) / SPHERE_TRANSITION_STEP;
                    } 
                    setRGB0(p, b, b, b);
                    setRGB2(p, bb, bb, bb);
                    setRGB1(p, bbb, bbb, bbb);
                    setRGB3(p, bbbb, bbbb, bbbb);
                }                
            }
        }        
        display();
        t += dt;
    }    
    MemoryManager::free(sphere_vector);    
    MemoryManager::free(poly_sphere);    
}
