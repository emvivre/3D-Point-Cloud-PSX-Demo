#include <sys/types.h>
#include "mm.h"
#include "util.h"

#if 0
#include <malloc.h>


namespace HeapUtil
{
    inline void init(unsigned long addr = 0x800F0000, long sz = 1*1024*1024) {
        InitHeap3((unsigned long*)addr, sz);
    }
};
#endif

static void* heap_base;
static void* heap_end;
static void* heap_limit;

namespace MemoryManager
{
    void init(unsigned int hb, unsigned int hl) 
    {
        heap_base = (void*)hb;
        heap_limit = (void*)hl;
        free_all();
    }
    
    void free_all()
    {
        heap_end = heap_base;
    }
    
    void free(void*) {}
    
    void* malloc(int sz) 
    {
        if ( sz > left() ) {
            //DISPLAY_PRINT_AND_EXIT("Unable to allocate %d bytes\n", sz);
            return 0;
        }
        void* p = heap_end;
        heap_end = (unsigned char*)heap_end + sz;
        return p;
    }
    
    int size() 
    {
        return (unsigned char*)heap_end - (unsigned char*)heap_base;
    }
    
    int left()
    {
        return (unsigned char*)heap_limit - (unsigned char*)heap_end;
    }
}