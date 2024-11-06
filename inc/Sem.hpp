#include "../h/list.hpp"
#include "../h/tcb.hpp"


class Sem;
typedef Sem* sem_t;

class Sem {
public:
    friend TCB;
    static Sem* createSem(unsigned init);

    static int sem_open (sem_t* handle, unsigned init);

    static int sem_close (sem_t handle);

    static int sem_wait (sem_t id);

    static int sem_signal (sem_t id);

private:

    Sem(unsigned init): init((int)init), open(true) {};

    int init;

    bool open;

    List<TCB> blocked;

};


