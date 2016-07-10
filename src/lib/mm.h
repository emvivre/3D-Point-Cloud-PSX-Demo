#ifndef _MM_H_
#define _MM_H_

namespace MemoryManager
{
    void init(unsigned int heap_base = 0x80100000, unsigned int heap_limit=0x801F0000);
    void* malloc(int sz);
    void free(void* p);
    void free_all();
    int size();
    int left();
};

#endif