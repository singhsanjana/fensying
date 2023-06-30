#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"
#define LOCKED   1
#define UNLOCKED 0
#define LOOP 1

typedef struct {
    atomic_int state;
} cas_lock;

static inline void cas_lock_init(cas_lock *lock)
{
    atomic_init(&lock->state,0);
}

static inline void cas_lock_acquire(cas_lock *lock){
    int expected = 0;
    for (int i=0;i<LOOP;i++)
    {
        if (atomic_compare_exchange_strong(__FILE__,__LINE__,&lock->state, &expected, 1)){
            break;
        }else{
            expected = 0;
            thrd_yield();
        }

    }
    
}

static inline void cas_lock_release(cas_lock *lock){
    atomic_store(__FILE__, __LINE__, &lock->state,0);
}

cas_lock lock;

atomic_int sd1;
atomic_int sd2;
static void tf1(void *arg) {
    // ticket_lock_t *lock = (ticket_lock_t *)arg;
    int i;

    for (i = 0; i < 2; i++) {
        cas_lock_acquire(&lock);
        atomic_store_explicit(__FILE__, __LINE__, &sd1, 17, memory_order_seq_cst);
        int a = atomic_load_explicit(__FILE__, __LINE__, &sd1, memory_order_seq_cst);
        MODEL_ASSERT(a==17);
        cas_lock_release(&lock);
    }

}
static void tf2(void *arg) {
    // ticket_lock_t *lock = (ticket_lock_t *)arg;
    int i;

    for (i = 0; i < 2; i++) {
        cas_lock_acquire(&lock);
        atomic_store_explicit(__FILE__, __LINE__, &sd1, 34, memory_order_seq_cst);
        int b = atomic_load_explicit(__FILE__, __LINE__, &sd1, memory_order_seq_cst);
        MODEL_ASSERT(b==34);
        cas_lock_release(&lock);
    }

}


int user_main(int argc, char **argv){
    cas_lock_init(&lock);
    thrd_t threads[2];
    atomic_init(&sd1,0);
    atomic_init(&sd2,0);
    // for (int i = 0; i < 2; i++) {
    thrd_create(&threads[0], (thrd_start_t)tf1, NULL);
    thrd_create(&threads[1], (thrd_start_t)tf2, NULL);
    // }

    for (int i = 0; i < 2; i++) {
        thrd_join(threads[i]);
    }


    return 0;
}
