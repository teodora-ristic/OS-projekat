#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"


Thread::Thread(void (*body)(void *), void *arg){
    this->body = body;
    this->arg = arg;
}


int Thread::start() {
    return thread_create(&myHandle,body,arg);
}


void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::~Thread() {
    delete myHandle;
}

Thread::Thread() {
    this->body = &wrapper;
    this->arg = this;
}

int Thread::sleep(time_t) {
    return 0;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

void PeriodicThread::terminate() {

}

PeriodicThread::PeriodicThread(time_t period) {

}
