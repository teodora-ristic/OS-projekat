#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"


MemoryAllocator::MemBlock* MemoryAllocator::freeMemHead = (MemoryAllocator::MemBlock*) HEAP_START_ADDR;

void *MemoryAllocator::mem_alloc(size_t size) {

    //Size in blocks

    if (!freeMemHead) {
        freeMemHead = (MemBlock*)HEAP_START_ADDR;
        freeMemHead->next = nullptr;
        freeMemHead->prev = nullptr;
        freeMemHead->size = (size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR - MEM_BLOCK_SIZE;
    }

    MemBlock *cur = freeMemHead;
    for (; cur!=nullptr; cur=cur->next)
        if (cur->size >= size*MEM_BLOCK_SIZE) break;


    // No free space
    if (cur == nullptr) {
        return nullptr;
    }

    // Allocate the requested block:
    size_t remainingSize = cur->size - size * MEM_BLOCK_SIZE;
    if (remainingSize >= 2*MEM_BLOCK_SIZE) {
        // A fragment remains
        size_t sz = size * MEM_BLOCK_SIZE;
        MemBlock* newBlk = (MemBlock*)((char*)cur + MEM_BLOCK_SIZE + sz);

        if (cur->prev) cur->prev->next = newBlk;
        else freeMemHead = newBlk;
        if (cur->next) cur->next->prev = newBlk;
        newBlk->prev = cur->prev;
        newBlk->next = cur->next;
        newBlk->size = remainingSize - MEM_BLOCK_SIZE;

        cur->size = sz;
    } else {
        // No remaining fragment, allocate the entire block
        if (cur->prev) cur->prev->next = cur->next;
        else freeMemHead = cur->next;
        if (cur->next) cur->next->prev = cur->prev;
    }
    cur->next = cur->prev = nullptr;
    return (char*)cur + MEM_BLOCK_SIZE;
}

int MemoryAllocator::tryToJoin(MemoryAllocator::MemBlock *cur)  {

    if (!cur) return 0;
    if (cur->next && (size_t)((char*)cur)+cur->size+MEM_BLOCK_SIZE == (size_t )(char*)(cur->next)) {
        // Remove the cur->next segment:
        cur->size += cur->next->size + MEM_BLOCK_SIZE;
        cur->next = cur->next->next;
        if (cur->next) cur->next->prev = cur;
        return 1;
    } else
        return 0;
}

int MemoryAllocator::mem_free(void *addr) {

    //if (addr == nullptr || addr < HEAP_START_ADDR || addr > HEAP_END_ADDR) return -1;
    // Find the place where to insert the new free segment (just after cur):
    MemBlock* ptr = (MemBlock*)((char*)addr - MEM_BLOCK_SIZE);
    MemBlock* cur=0;
    if (!freeMemHead || (char*)ptr<(char*)freeMemHead)
        cur = nullptr; // insert as the first
    else
        for (cur=freeMemHead; cur->next!= nullptr && (char*)ptr>(char*)(cur->next); cur=cur->next);
    // Insert the new segment after cur:
    MemBlock* newSeg = (MemBlock*)((char*)ptr);
    newSeg->size = getSize(ptr);
    newSeg->prev = cur;
    if (cur) newSeg->next = cur->next;
    else newSeg->next = freeMemHead;
    if (newSeg->next) newSeg->next->prev = newSeg;
    if (cur) cur->next = newSeg;
    else freeMemHead = newSeg;
    // Try to merge with the previous and next segments:
    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;




}


/*
void *MemoryAllocator::mem_alloc(size_t size) {

    //Size in blocks

    if (!freeMemHead) {
        freeMemHead = (MemBlock*)HEAP_START_ADDR;
        freeMemHead->next = nullptr;
        freeMemHead->prev = nullptr;
        freeMemHead->size = (size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR - MEM_BLOCK_SIZE;
    }


    MemBlock *blk = freeMemHead;
    MemBlock *prev = nullptr;
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk->size>=size*MEM_BLOCK_SIZE) break;


    // If not found, allocate a new memory segment and add it to the list:
    if (blk == nullptr) {
        size_t sz = size*MEM_BLOCK_SIZE;
        blk = (MemBlock*)((char*)prev+MEM_BLOCK_SIZE+prev->size);
        if ((size_t)((char*)prev+prev->size+MEM_BLOCK_SIZE+sz+MEM_BLOCK_SIZE) > (size_t)HEAP_END_ADDR) return nullptr; // No free memory
        prev->next = blk;
        blk->prev = prev;
        blk->next= nullptr;
        blk->size = sz;
    }

    // Allocate the requested block:
    size_t remainingSize = blk->size - size * MEM_BLOCK_SIZE;
    if (remainingSize >= 2*MEM_BLOCK_SIZE) {
        // A fragment remains
        blk->size = size * MEM_BLOCK_SIZE;
        size_t offset = MEM_BLOCK_SIZE + size * MEM_BLOCK_SIZE;
        MemBlock* newBlk = (MemBlock*)((char*)blk + offset);
        if (prev) {prev->next = newBlk; newBlk->prev = prev;}
        else freeMemHead = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize - MEM_BLOCK_SIZE;
    } else {
        // No remaining fragment, allocate the entire block
        if (prev) {prev->next = blk->next; blk->next->prev = prev;}
        else freeMemHead = blk->next;
    }
    blk->next = blk->prev = nullptr;
    return (char*)blk;
}

int MemoryAllocator::tryToJoin(MemoryAllocator::MemBlock *cur)  {

    if (!cur) return 0;
    if (cur->next && (size_t)((char*)cur)+cur->size+MEM_BLOCK_SIZE == (size_t )(char*)(cur->next)) {
        // Remove the cur->next segment:
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next) cur->next->prev = cur;
        return 1;
    } else
        return 0;
}

int MemoryAllocator::mem_free(void *addr) {

// Find the place where to insert the new free segment (just after cur):

    MemBlock* cur=0;
    if (!freeMemHead || addr<(char*)freeMemHead)
        cur = nullptr; // insert as the first
    else
        for (cur=freeMemHead; cur->next!= nullptr && addr>(char*)(cur->next);
             cur=cur->next);
    // Insert the new segment after cur:
    MemBlock* newSeg = (MemBlock*)((char*)addr);
    newSeg->size = getSize(addr);
    newSeg->prev = cur;
    if (cur) newSeg->next = cur->next;
    else newSeg->next = freeMemHead;
    if (newSeg->next) newSeg->next->prev = newSeg;
    if (cur) cur->next = newSeg;
    else freeMemHead = newSeg;
    // Try to merge with the previous and next segments:
    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}

*/


