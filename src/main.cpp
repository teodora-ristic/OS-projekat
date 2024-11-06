#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/userMain.h"
// "../h/MemoryAllocator.hpp"

int main(){

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap|1);

    TCB *main[2];
    thread_create(&main[0], nullptr, nullptr);
    TCB::running = main[0];

    thread_create(&main[1], reinterpret_cast<void (*)(void *)>(userMain), nullptr);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(main[1]->isFinished()))
    {
        thread_dispatch();
    }

    for (auto &thread: main)
    {
        delete thread;
    }

    printString("Finished\n");
    return 0;


/*
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap|1);

    TCB *main;
    thread_create(&main, nullptr, nullptr);
    TCB::running = main;

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    userMain();

    printString("Finished\n");
    return 0;*/

}
