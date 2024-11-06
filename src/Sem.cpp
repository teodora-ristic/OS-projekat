#include "../h/Sem.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.h"



Sem *Sem::createSem(unsigned init) {
    return new Sem(init);
}



int Sem::sem_open(sem_t *handle, unsigned init) {
    *handle = createSem(init);
    if (*handle) return 0;
    else return -1;
}

int Sem::sem_close(sem_t handle) {
    if (!handle->open) return -1;
    for (thread_t t = handle->blocked.removeFirst(); t != nullptr ; t = handle->blocked.removeFirst() )
    {
        t->setBlocked(false);
        Scheduler::put(t);
    }
    handle->open = false;
    delete handle;
    return 0;
}

int Sem::sem_wait(sem_t id) {
    if (!id->open) return -1;
    if (--id->init<0){
        TCB::running->setBlocked(true);
        id->blocked.addLast(TCB::running);
        thread_dispatch();
    }
    //thread_dispatch();
    return 0;
}

int Sem::sem_signal(sem_t id) {
    if (!id->open) return -1;
    if (++id->init<=0){
        TCB* t = id->blocked.removeFirst();
        t->setBlocked(false);
        Scheduler::put(t);
    }
    //thread_dispatch();
    return 0;


}

