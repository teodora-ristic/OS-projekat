//#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"


using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    //return __mem_alloc(n);
    size_t block = (n/ MEM_BLOCK_SIZE) + ( (n % MEM_BLOCK_SIZE) > 0 ? 1 : 0);
    return MemoryAllocator::mem_alloc(block);
}

void *operator new[](size_t n)
{
    //return __mem_alloc(n);
    size_t block = (n/ MEM_BLOCK_SIZE) + ( (n % MEM_BLOCK_SIZE) > 0 ? 1 : 0);
    return MemoryAllocator::mem_alloc(block);
}

void operator delete(void *p) noexcept
{
    //__mem_free(p);
    MemoryAllocator::mem_free(p);
}

void operator delete[](void *p) noexcept
{
    //__mem_free(p);
    MemoryAllocator::mem_free(p);
}

