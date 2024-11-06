#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"


TCB *TCB::running = nullptr;

//uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg, void* stack_space)
{
    return new TCB(body, arg, stack_space);
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!(old->isFinished()) && !(old->isBlocked())) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    running->releaseJoined();
    ::thread_dispatch();

}

int TCB::thread_create(thread_t *handle, void (*start_routine)(void *), void *arg,  void* stack_space) {
     *handle = createThread(start_routine, arg, stack_space);
     if (*handle == nullptr) return -1;
     else return 0;
}

int TCB::thread_exit() {
    running->setFinished(true);
    ::thread_dispatch();
    return 0;

}

void  TCB::thread_dispatch() {
    dispatch();
}

void TCB::thread_join(thread_t handle) {
    if(handle->isFinished()) {
        running->setBlocked(true);
        handle->joined.addLast(running);
    }

}

void TCB::releaseJoined() {
    for (thread_t t = this->joined.removeFirst(); t != nullptr ; t = this->joined.removeFirst() )
    {
        t->setBlocked(false);
        Scheduler::put(t);
    }
}


