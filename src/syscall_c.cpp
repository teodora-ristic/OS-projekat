#include "../lib/hw.h"
#include "../h/syscall_c.h"
//#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"


void* mem_alloc (size_t size) {
    uint64 opcode = 0x01;
    uint64 block = (size / MEM_BLOCK_SIZE) + ( (size % MEM_BLOCK_SIZE) > 0 ? 1 : 0);
    __asm__ volatile ("mv a1, %[size]" : : [size] "r"(block));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    void* ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;

}


int mem_free (void* ptr) {
    uint64 opcode = 0x02;
    __asm__ volatile ("mv a1, %[pt]" : : [pt] "r"(ptr));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    uint64 opcode = 0x11;
    size_t block = (DEFAULT_STACK_SIZE/ MEM_BLOCK_SIZE);
    uint64* stack_space =(uint64*)MemoryAllocator::mem_alloc(block*sizeof(uint64));
    //uint64* stack_space =(uint64*)__mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));
    __asm__ volatile ("mv a7, %[s]" : : [s] "r"(stack_space));
    __asm__ volatile ("mv a6, %[arg]" : : [arg] "r"(arg));
    __asm__ volatile ("mv a2, %[sr]" : : [sr] "r"(start_routine));
    __asm__ volatile ("mv a1, %[h]" : : [h] "r"(handle));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int thread_exit() {
    uint64 opcode = 0x12;
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void thread_dispatch() {
    uint64 opcode = 0x13;
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle) {
    uint64 opcode = 0x14;
    __asm__ volatile ("mv a1, %[h]" : : [h] "r"(handle));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
}

char getc (){
    uint64 opcode = 0x41;
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    char ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void putc (char c) {
    uint64 opcode = 0x42;
    char ch = c;
    __asm__ volatile ("mv a1, %[c]" : : [c] "r"(ch));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned init) {
    uint64 opcode = 0x21;
    __asm__ volatile ("mv a2, %[i]" : : [i] "r"(init));
    __asm__ volatile ("mv a1, %[h]" : : [h] "r"(handle));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_close(sem_t handle) {
    uint64 opcode = 0x22;
    __asm__ volatile ("mv a1, %[h]" : : [h] "r"(handle));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_wait(sem_t id) {
    uint64 opcode = 0x23;
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    uint64 opcode = 0x24;
    __asm__ volatile ("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile ("mv a0, %[op]" : : [op] "r"(opcode));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

int time_sleep(time_t) {
    return 0;
}

