#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

#define UNLOCKED 0
#define WRITE_LOCKED 1
#define READ_LOCKED 2
#define LOOP1 1
#define LOOP2 1
#define LOOP3 1

typedef struct {
    atomic_int state;
    atomic_int num_readers;
} rw_lock;

static inline void rw_lock_init(rw_lock *lock)
{
    // lock->state = UNLOCKED;
    atomic_init(&lock->state, 0);
    atomic_init(&lock->num_readers, 0);
    // lock->num_readers = 0;
}

static inline int rw_lock_acquire_read(rw_lock *lock)
{
    int expected = UNLOCKED;
    for (int i=0;i<LOOP1;i++) {
        if (atomic_compare_exchange_strong_explicit(__FILE__,__LINE__,&lock->state, &expected, READ_LOCKED, memory_order_seq_cst, memory_order_seq_cst)){
            atomic_fetch_add(__FILE__,__LINE__,&lock->num_readers, 1);
            return 1;
        }   
        
        if (expected == READ_LOCKED) {
            atomic_fetch_add(__FILE__,__LINE__,&lock->num_readers, 1);
            return 1;
        } 

        expected = UNLOCKED;
    }
    
    return 0;
    // atomic_store_explicit(__FILE__,__LINE__,&lock->state, UNLOCKED,memory_order_relaxed);
}

static inline void rw_lock_release_read(rw_lock *lock)
{
    atomic_fetch_sub(__FILE__,__LINE__,&lock->num_readers, 1);
}

static inline int rw_lock_acquire_write(rw_lock *lock)
{
    int expected = UNLOCKED;
    for (int i=0;i<LOOP1;i++) {
        if (atomic_compare_exchange_strong_explicit(__FILE__,__LINE__,&lock->state, &expected, WRITE_LOCKED, memory_order_seq_cst, memory_order_seq_cst)){
            return 1;
        }

        if (expected == READ_LOCKED)
            break;

        expected = UNLOCKED;
    }

    if (expected != READ_LOCKED)
        return 0;

    for (int i=0;i<LOOP2;i++) {
        int r = atomic_load_explicit(__FILE__,__LINE__,&lock->num_readers,memory_order_seq_cst);
        
        if (r <= 0) {
            return 1;
        }
    }

    return 0;
}

static inline void rw_lock_release_write(rw_lock *lock)
{
    atomic_store_explicit(__FILE__,__LINE__,&lock->state, UNLOCKED,memory_order_seq_cst);
}
rw_lock lock;
atomic_int sd1;
atomic_int sd2;

static void fn1(void *obj){
    if (rw_lock_acquire_read(&lock)) {
        int a = atomic_load_explicit(__FILE__, __LINE__, &sd2, memory_order_relaxed);
        int b = atomic_load_explicit(__FILE__, __LINE__, &sd1, memory_order_relaxed);
        MODEL_ASSERT(a == 0 || b == 1);
        rw_lock_release_read(&lock);
    }
}

static void fn2(void *obj){
    if (rw_lock_acquire_write(&lock)) {
        atomic_store_explicit(__FILE__, __LINE__, &sd1, 1, memory_order_relaxed);
        atomic_store_explicit(__FILE__, __LINE__, &sd2, 1, memory_order_relaxed);
        rw_lock_release_write(&lock);
    }
}

int user_main(int argc, char **argv)
{
    rw_lock_init(&lock);

    thrd_t threads[4];
    
    atomic_init(&sd1, 0);
    atomic_init(&sd2, 0);
    
    // Create two reader threads
    for (int i = 0; i < 2; i++) {
        thrd_create(&threads[i], (thrd_start_t)fn1, NULL);
    }

    // Create two writer threads
    for (int i = 2; i < 4; i++) {
        thrd_create(&threads[i], (thrd_start_t)fn2, NULL);
    }

    // Join all threads
    for (int i = 0; i < 4; i++) {
        thrd_join(threads[i]);
    }

    // Release lock
    rw_lock_release_write(&lock);

    return 0;
}
