#ifndef _TCB_HPP
#define _TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

// Thread Control Block

typedef TCB* thread_t;

class TCB
{
public:
    using Body = void (*)(void*);

    static TCB* createThread(Body body, void* arg, void* stack_space);

    ~TCB() {
        delete[] stack;}

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }


    static void yield();

    static TCB* running;

    static int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);

    static int thread_exit ();

    static void thread_dispatch ();

    static void thread_join (thread_t handle);

/*
    void* operator new(size_t size){
        return MemoryAllocator::mem_alloc(size);
    }

    void* operator new[](size_t size){
        return MemoryAllocator::mem_alloc(size);
    }

    void operator delete (void *p) {
        MemoryAllocator::mem_free(p);
    }

    void operator delete[] (void *p) {
        MemoryAllocator::mem_free(p);
    }
*/

private:
    TCB(Body body, void* arg, __attribute__((unused)) void* stack_space) :
            body(body),
            arg(arg),
            stack(body != nullptr ? (uint64*) stack_space: nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE]: 0
                    }),
            finished(false),
            blocked(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }




    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool finished;
    bool blocked;
    List<TCB> joined;


    friend class Riscv;

    static void threadWrapper();

    void releaseJoined();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 constexpr STACK_SIZE = 1024;
};

#endif //_TCB_HPP
