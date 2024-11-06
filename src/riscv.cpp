#include "../h/riscv.hpp"
#include "../lib/console.h"
//#include "../h/MemoryAllocator.hpp"
#include "../h/Sem.hpp"
#include "../test/printing.hpp"

//class TCB;
typedef TCB* thread_t;

typedef Sem* sem_t;


void Riscv::popSppSpie()
{

    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleIntr() {
    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 opcode;
        __asm__ volatile ("mv %[op], a0" : [op] "=r"(opcode));
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        if (opcode == 0x01) {


            size_t size;
            __asm__ volatile ("mv %[sz], a1" : [sz] "=r"(size));
            //void* i = __mem_alloc(size);
            void *i = MemoryAllocator::mem_alloc(size);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x02) {

            void *ptr;
            __asm__ volatile ("mv %[pt], a1" : [pt] "=r"(ptr));
            //int i = __mem_free(ptr);
            int i = MemoryAllocator::mem_free(ptr);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x11) {

            thread_t *handle;
            void (*start_routine)(void *);
            void *arg;
            void *stack_space = nullptr;

            __asm__ volatile ("mv %[h], a1" : [h] "=r"(handle));
            __asm__ volatile ("mv %[sr], a2" : [sr] "=r"(start_routine));
            __asm__ volatile ("mv %[arg], a6" : [arg] "=r"(arg));
            __asm__ volatile ("mv %[s], a7" : [s] "=r"(stack_space));

            int i = TCB::thread_create(handle, start_routine, arg, stack_space);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x12) {

            int i = TCB::thread_exit();
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");

        } else if (opcode == 0x13) {

            TCB::thread_dispatch();

        } else if (opcode == 0x14) {

            thread_t handle;
            __asm__ volatile ("mv %[h], a1" : [h] "=r"(handle));

            TCB::thread_join(handle);

        } else if (opcode == 0x41) {

            char c = __getc();
            __asm__ volatile ("mv a0, %[c]" : : [c] "r"(c));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x42) {

            char c;
            __asm__ volatile ("mv %[c], a1" : [c] "=r"(c));
            __putc(c);


        } else if (opcode == 0x21) {

            sem_t *handle;
            unsigned init;

            __asm__ volatile ("mv %[h], a1" : [h] "=r"(handle));
            __asm__ volatile ("mv %[sr], a2" : [sr] "=r"(init));

            int i = Sem::sem_open(handle, init);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x22) {

            sem_t handle;
            __asm__ volatile ("mv %[h], a1" : [h] "=r"(handle));

            int i = Sem::sem_close(handle);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");

        } else if (opcode == 0x23) {

            sem_t id;
            __asm__ volatile ("mv %[id], a1" : [id] "=r"(id));

            int i = Sem::sem_wait(id);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else if (opcode == 0x24) {

            sem_t id;
            __asm__ volatile ("mv %[id], a1" : [id] "=r"(id));

            int i = Sem::sem_signal(id);
            __asm__ volatile ("mv a0, %[i]" : : [i] "r"(i));

            __asm__ volatile ("sd a0, 80(s0)");


        } else { //yield
            TCB::thread_dispatch();
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else {
        printInt(Riscv::r_scause());
        while (1);
        // unexpected trap cause
    }
}


void Riscv::handleTimer() {
    uint64 scause = r_scause();
    if (scause == 0x8000000000000001UL)
    {

        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    }

    else
    {
        printInt(Riscv::r_scause());
        while (1);
        // unexpected trap cause
    }
}

void Riscv::handleConsole() {
    uint64 scause = r_scause();
    if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        printInt(Riscv::r_scause());
        while (1);
        // unexpected trap cause
    }
}