#include "../lib/hw.h"


class MemoryAllocator {
public:
    static void* mem_alloc (size_t size);

    static int mem_free(void* addr);

    static size_t getSize(void *ptr) {
        return ((MemBlock*)ptr)->size;
    }
protected:
    //MemoryAllocator() {};

    struct MemBlock {
        MemBlock* next; // Next in the list
        MemBlock* prev; // Previous in the list
        size_t size; // Size of the free fragment
    };

    static MemBlock* freeMemHead;



private:
    static int tryToJoin(MemoryAllocator::MemBlock *cur);







};