#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libmath.h>
#include <stdlib.h>
#include <string.h>
#include <libcd.h>
#include <libsnd.h>
#include "pixelfb.h"
#include "util.h"
#include "common.h"
#include "mm.h"

extern const int OT_LENGTH = 4000;

struct DB {    
    DRAWENV draw;
    DISPENV disp;
    u_long ot[OT_LENGTH];
};
static DB db[2];
static DB* cdb;
u_long* ot;

unsigned long __ramsize =   0x002000000; // force the PSX to use 2 megabytes of RAM
unsigned long __stacksize = 0x00004000; // force the PSX to use 16 kilobytes of stack

static void graphics_init();

#define TEXTURE_WIDTH_MAX 256

extern void intro_msg();
extern void cube_dissolve();
extern void sphere();
extern void ptcloud();
extern void credits();

void set_clear_color(unsigned char i)
{
    set_clear_color(i, i, i);
}

void enable_auto_clear_color_buffer() 
{
    clear_color_buffer();    
}

void disable_auto_clear_color_buffer()
{
    db[0].draw.isbg = 0;
    db[1].draw.isbg = 0;    
}

void set_clear_color(unsigned char r, unsigned char g, unsigned char b)
{
    db[0].draw.r0 = r;    
    db[0].draw.g0 = g; 
    db[0].draw.b0 = b;  
    db[1].draw.r0 = r;    
    db[1].draw.g0 = g; 
    db[1].draw.b0 = b; 
}

void clear_color_buffer()
{
    db[0].draw.isbg = 1;
    db[1].draw.isbg = 1;
}

#define SECTOR_DATA_SIZE 2048
int xa_file_beg;
int xa_file_end;
int xa_current_channel = 0;
static void play_beginning()
{
    CdlFILTER filter;
    filter.file = 1;
    filter.chan = xa_current_channel;
    CdControlF(CdlSetfilter, (u_char*)&filter);
    CdlLOC loc;
    CdIntToPos(xa_file_beg, &loc);
    CdControlF(CdlReadS, (u_char*)&loc);
}
static void cbready(unsigned char status, unsigned char*)
{
    if ( status == CdlDataReady ) {
        CdlLOC loc;        
        CdGetSector(&loc, sizeof(loc));
        int sector_id = CdPosToInt(&loc);
        if ( sector_id >= xa_file_end ) {
            xa_current_channel ^= 1;
            play_beginning();            
        }
    }
}

static void init_music()
{
    CdlFILE cdf;        
    CdSetDebug(3);     
    if ( CdSearchFile(&cdf, "\\MUSIC.XA;1") == NULL ) {    
        DEBUG_PRINT_AND_EXIT("Unable to find music \\MUSIC.XA file !");
    }            
    xa_file_beg = CdPosToInt(&cdf.pos);
    xa_file_end = xa_file_beg + cdf.size / SECTOR_DATA_SIZE;
    u_char setmode[4] = { CdlModeSF | CdlModeRT | CdlModeSize1 };
    CdControlB(CdlSetmode, setmode, NULL);
    SsSetTickMode( PsxUtil::is_pal() ? SS_TICK50 : SS_TICK60);
    SsSetSerialVol(SS_SERIAL_A, 127, 127);
    CdControlF(CdlPause, NULL);
    CdReadyCallback(cbready);    
    play_beginning();        
}    

int main()
{    
    CdInit();
    graphics_init();    
    MemoryManager::init(0x800E0000);    
    init_music();
    intro_msg(); MemoryManager::free_all();
    cube_dissolve(); MemoryManager::free_all();
    sphere(); MemoryManager::free_all();    
    ptcloud(); MemoryManager::free_all();
    credits(); MemoryManager::free_all();
    return 0;
}

static void graphics_init()
{
    ResetGraph(0);
    SetGraphDebug(false);
    InitGeom();
    SetGeomOffset(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SetGeomScreen(1000);
    db[0].disp.screen.x = db[1].disp.screen.x = 0;         
    if ( PsxUtil::is_pal() ) {
        SetVideoMode(MODE_PAL);
        db[0].disp.screen.y = db[1].disp.screen.y = 28;        
    } else {
        SetVideoMode(MODE_NTSC);
        db[0].disp.screen.y = db[1].disp.screen.y = 0;        
    }
    db[0].disp.screen.w = db[1].disp.screen.w = SCREEN_WIDTH;
    db[0].disp.screen.h = db[1].disp.screen.h = SCREEN_HEIGHT;
    for (int i = 0; i < 2; i++) {
        SetDefDispEnv(&db[i].disp, 0, i*SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
        SetDefDrawEnv(&db[i].draw, 0, (1-i)*SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
        db[i].draw.dtd = 0;
    }
    FntLoad(960, 256);
    SetDumpFnt(FntOpen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DONT_CLEAR_BACKGROUND, 512));
    SetDispMask(true);
    cdb = &db[0];
    PutDrawEnv(&cdb->draw);
    PutDispEnv(&cdb->disp);
    ot = cdb->ot;
    ClearOTag(ot, OT_LENGTH);
}

void display()
{
    FntFlush(-1);
    DrawSync(0);
    VSync(0);
    cdb = (cdb == &db[0]) ? &db[1] : &db[0];
    PutDrawEnv(&cdb->draw);
    PutDispEnv(&cdb->disp);
    DrawOTag(ot);
    ot = cdb->ot;
    ClearOTag(ot, OT_LENGTH);
}