#include "common.h"
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>
#include "pixelfb.h"
#include "bishop.h"
#include "trigo.h"
#include <stdio.h>
#include "mm.h"

#ifndef NULL
#define NULL 0
#endif

struct DB {  
    TILE_1* pc;
};
static void init_prim(DB* d);
static DB* cdb;

class PointCloudAnimation
{        
    struct Scene {
        virtual void init() = 0;
        virtual bool update() = 0; // return true if continue in same scene
    };    
    class SceneRotation360 : public Scene {
        int rot;   
        void init() {            
            rot = DEG2RAD(360);
            VECTOR trans = {0, 0, 50000};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);
        } 
        bool update() {
            clear_color_buffer();
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            rot-=DEG2RAD(10);
            return rot > DEG2RAD(0) ? true : false;
        }
    };
    class SceneRotation : public Scene {
        int rot;   
        void init() {            
            rot = DEG2RAD(360);
            VECTOR trans = {0, 0, 50000};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);
        } 
        bool update() {
            clear_color_buffer();
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            rot-=DEG2RAD(10);
            return rot > DEG2RAD(200) ? true : false;
        }
    };
    class SceneRotationZoom0 : public Scene {
        int rot;   
        int z;
        int x;
        void init() {                        
            rot = DEG2RAD(200);
            z = 50000;    
            x = 0;            
        } 
        bool update() {
            clear_color_buffer();
            #define SCENE_ROTATION_SCALE 6/10
            VECTOR trans = {x, 0, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);            
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            rot+=DEG2RAD(3 * SCENE_ROTATION_SCALE);
            z -= 2000 * SCENE_ROTATION_SCALE;            
            x += 300 * SCENE_ROTATION_SCALE;            
            return z > 300 ? true : false;
        }
    };
    class SceneRotationZoom1 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {                        
            rot = DEG2RAD(250);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = 100;
        } 
        bool update() {   
            clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= z*6/100;            
            return z > 1000 ? true : false;
        }
    };
    class SceneRotationZoom2 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {                        
            rot = DEG2RAD(200);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = -100;
        } 
        bool update() {   
            clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;
            return z > 1000 ? true : false;
        }
    };
    class SceneRotationZoom3 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {                        
            rot = DEG2RAD(300);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = 15000;
        } 
        bool update() {    
            clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;
            y -= 1000;
            return y > -6000 ? true : false;
        }
    };
    class SceneRotationZoom4 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {                        
            rot = DEG2RAD(250);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 10000;       
            y = 1000;
        } 
        bool update() {            
            clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;            
            return z > 100 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {        
            enable_auto_clear_color_buffer(); display(); display(); display();
            rot = DEG2RAD(250);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 5000;       
            y = 1000;
        } 
        bool update() {   
            disable_auto_clear_color_buffer();   
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 1000;            
            return z > 100 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom0 : public Scene {
        int rot;   
        int z;
        int x;
        void init() {      
            enable_auto_clear_color_buffer(); display(); display(); display();            
            rot = DEG2RAD(200);
            z = 50000;    
            x = 0;            
        } 
        bool update() {            
            disable_auto_clear_color_buffer();
            #define SCENE_ROTATION_SCALE 6/10
            VECTOR trans = {x, 0, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);            
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            rot+=DEG2RAD(3 * SCENE_ROTATION_SCALE);
            z -= 2000 * SCENE_ROTATION_SCALE;            
            x += 300 * SCENE_ROTATION_SCALE;            
            return z > 300 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom1 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {   
            enable_auto_clear_color_buffer(); display(); display(); display();
            rot = DEG2RAD(250);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = 100;
        } 
        bool update() {   
            disable_auto_clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= z*6/100;            
            return z > 1000 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom2 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {        
            enable_auto_clear_color_buffer(); display(); display(); display();
            rot = DEG2RAD(200);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = -100;
        } 
        bool update() {   
            disable_auto_clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;
            return z > 1000 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom3 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {            
            enable_auto_clear_color_buffer(); display(); display(); display();          
            rot = DEG2RAD(300);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 0;       
            y = 15000;
        } 
        bool update() {    
            disable_auto_clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;
            y -= 1000;
            return y > -6000 ? true : false;
        }
    };
    class SceneRotationDisableClearingFBZoom4 : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {             
            enable_auto_clear_color_buffer(); display(); display(); display();
            rot = DEG2RAD(250);
            SVECTOR angl = { 0, rot, 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            z = 50000;    
            x = 10000;       
            y = 1000;
        } 
        bool update() {            
            disable_auto_clear_color_buffer();
            VECTOR trans = {x, y, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);                                    
            z -= 3000;            
            return z > 100 ? true : false;
        }
    };
    class SceneRotationTranslationRotX : public Scene {
        int rot;   
        int z;
        int x;
        int y;
        void init() {                        
            rot = DEG2RAD(0);
            z = 50000;    
            x = 0;     
            y = 0;
        } 
        bool update() {
            clear_color_buffer();           
            VECTOR trans = {x, 0, z};            
            TransMatrix(&PointCloudAnimation::m, &trans);
            SetTransMatrix(&PointCloudAnimation::m);            
            SVECTOR angl = { rot, DEG2RAD(250), 0 };
            RotMatrix(&angl, &PointCloudAnimation::m);
            SetRotMatrix(&PointCloudAnimation::m);
            rot+=DEG2RAD(1);
            z -= 1000;          
            // y -= 1000;
            return z > 1000 ? true : false;
        }
    };    
    static Scene* scenes[]; // scenes knownledge / database        
    int current_scene_i;
    bool is_end;
    bool is_continue;
public:
    static MATRIX m;
    PointCloudAnimation() {
        current_scene_i = 0;        
        init();
        is_end = false;
        is_continue = true;
    }
    bool is_finish() const { return is_end; }
    void init() {
        if ( scenes[current_scene_i] == NULL ) {
            is_end = true;      
            return ;
        }
        scenes[current_scene_i]->init();        
    }
    void update() {                   
        if ( is_continue == false ) {
            current_scene_i++;
            init();            
        }        
        if ( scenes[current_scene_i] == NULL ) return ;
        is_continue = scenes[current_scene_i]->update();        
    }
};
MATRIX PointCloudAnimation::m;
 PointCloudAnimation::Scene* PointCloudAnimation::scenes[] = {       
    new PointCloudAnimation::SceneRotation360(),        
    new PointCloudAnimation::SceneRotation(),
    new PointCloudAnimation::SceneRotationZoom0(),
    new PointCloudAnimation::SceneRotationZoom1(),
    new PointCloudAnimation::SceneRotationZoom2(),
    new PointCloudAnimation::SceneRotationZoom3(),
    new PointCloudAnimation::SceneRotationZoom4(),        
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom(),    
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom0(),
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom1(),
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom2(),
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom3(),
    new PointCloudAnimation::SceneRotationDisableClearingFBZoom4(),   
    new SceneRotationTranslationRotX(),  
    NULL
 };

void ptcloud()
{
    SetGeomScreen(300);
    DB db[2]; 
    cdb = &db[0];
    int t = 0;    
    init_prim(&db[0]);
    init_prim(&db[1]);    
    set_clear_color(0);
    enable_auto_clear_color_buffer();
    PointCloudAnimation pa;
    pa.init();        
    while (1)
    {
        pa.update();
        if ( pa.is_finish() == true) break;
        long dmy, flag;
        TILE_1* p = cdb->pc;
        SVECTOR* s = bishop_point_cloud;        
        for ( int i = 0; i < BISHOP_NB_POINTS; i++, s++, p++ ) {               
            RotTransPers(s, (long*)&p->x0, &dmy, &flag);
            AddPrim(ot, p);
        }
        display();
        t++;
        cdb = cdb == &db[0] ? &db[1] : &db[0];
    }    
    MemoryManager::free(db[0].pc);    
    MemoryManager::free(db[1].pc);        
}

static void init_prim(DB* d)
{    
    d->pc = (TILE_1*)MemoryManager::malloc(BISHOP_NB_POINTS * sizeof(*d->pc));    
    TILE_1* p = d->pc;   
    u_char* c = &bishop_point_cloud_color[0][0];    
    for ( int i = 0; i < BISHOP_NB_POINTS; i++, p++, c+=3 ) {        
        setTile1(p);
        setRGB0(p, c[0], c[1], c[2]);
    }
}
