#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libapi.h>

#define DEBUG_PRINT(fmt, argv...) FntPrint(fmt, ## argv)
#define DEBUG_PRINT_AND_EXIT(fmt, argv...)              \
    do {                                                \
        display();                                      \
        cdb->draw.r0 = cdb->draw.g0 = cdb->draw.b0 = 0; \
        cdb->draw.isbg = 1;                             \
        DEBUG_PRINT(fmt, ## argv);                      \
        display();                                      \
        exit(0);                                        \
    } while(0)
    
namespace PsxUtil
{
    inline bool is_pal() { 
        return *(char *)0xbfc7ff52=='E'; 
    }
}

// PAD controller
#define PAD_CROSS PADRdown
#define PAD_TRIANGLE PADRup
#define PAD_SQUARE PADRleft
#define PAD_CIRCLE PADRright
#define PAD_UP PADLup
#define PAD_LEFT PADLleft
#define PAD_RIGHT PADLright
#define PAD_DOWN PADLdown
#define PAD_L1 PADL1
#define PAD_L2 PADL2
#define PAD_R1 PADR1
#define PAD_R2 PADR2
#define PAD_START PADstart
#define PAD_SELECT PADselect

namespace PadUtil
{
    inline void init() {
        PadInit(0);
    }
    inline unsigned int read(int player_id) {
        return PadRead(0) >> 16*player_id;
    }
}

namespace MCUtil
{
    inline void init() {
        InitCARD(0);
        StartCARD();
        _bu_init();
    }
    inline void stop() {
        StopCARD();
    }
}

#endif