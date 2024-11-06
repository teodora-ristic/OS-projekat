#include "../lib/hw.h"

class TCB;
typedef TCB* thread_t;

class Sem;
typedef Sem* sem_t;

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit ();

void thread_dispatch ();

void thread_join (thread_t handle);

char getc ();

void putc (char c) ;

int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

typedef unsigned long time_t;

int time_sleep (time_t);

