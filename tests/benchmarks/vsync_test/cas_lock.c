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

static inline int cas_lock_acquire(cas_lock *lock){
    int expected = 0;
    for (int i=0;i<LOOP;i++)
    {
        if (atomic_compare_exchange_strong_explicit(__FILE__,__LINE__, &lock->state, &expected, 1, memory_order_relaxed, memory_order_seq_cst))
            return 1;
            // break;

        expected = 0;
    }
    return 0;
}

static inline void cas_lock_release(cas_lock *lock){
    atomic_store_explicit(__FILE__, __LINE__, &lock->state, 0, memory_order_relaxed);
}

cas_lock lock;
atomic_int x;

static void tf1(void *arg) {
    for (int i = 0; i < 2; i++) {
        if (cas_lock_acquire(&lock)) {
            atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(a==1);
            cas_lock_release(&lock);
        }
    }

}
static void tf2(void *arg) {
    for (int i = 0; i < 2; i++) {
        if (cas_lock_acquire(&lock)) {
            atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(a==2);
            cas_lock_release(&lock);
        }
    }
}


int user_main(int argc, char **argv){
    cas_lock_init(&lock);
    thrd_t threads[2];
    atomic_init(&x, 0);

    // for (int i = 0; i < 2; i++) {
    thrd_create(&threads[0], (thrd_start_t)tf1, NULL);
    thrd_create(&threads[1], (thrd_start_t)tf2, NULL);
    // }

    for (int i = 0; i < 2; i++) {
        thrd_join(threads[i]);
    }


    return 0;
}
