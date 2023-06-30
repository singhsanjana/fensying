#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

#define LOOP 1

typedef union {
    atomic_int next_ticket;
    atomic_int now_serving;
} ticket_lock_t;

// void ticket_lock_init(ticket_lock_t *lock) {
//     atomic_init(&lock->next_ticket, 0);
//     atomic_init(&lock->now_serving, 0);
// }

static inline void ticket_lock_lock(ticket_lock_t *lock) {
    int my_ticket = atomic_fetch_add_explicit(__FILE__,__LINE__,&lock->next_ticket, 1,memory_order_relaxed);
    for( int i=0;i<LOOP;i++){
        if(atomic_load_explicit(__FILE__,__LINE__,&lock->now_serving,memory_order_relaxed)==my_ticket){
            break;
        }else{
            thrd_yield();
        }
    }
    // while (atomic_load_explicit(&lock->now_serving) != my_ticket) {
    //     // thrd_yield();
    // }
}

static inline void ticket_lock_unlock(ticket_lock_t *lock) {
    atomic_fetch_add_explicit(__FILE__,__LINE__,&lock->now_serving, 1,memory_order_relaxed);
}
ticket_lock_t lock;
atomic_int sd1;
static void tf1(void *arg) {
    // ticket_lock_t *lock = (ticket_lock_t *)arg;
    int i;

    for (i = 0; i < 1; i++) {
        ticket_lock_lock(&lock);
        atomic_store_explicit(__FILE__,__LINE__,&sd1,10,memory_order_relaxed);
        int a = atomic_load_explicit(__FILE__,__LINE__,&sd1, memory_order_relaxed);
        MODEL_ASSERT(a==10);
        ticket_lock_unlock(&lock);
    }

}
static void tf2(void *arg) {
    // ticket_lock_t *lock = (ticket_lock_t *)arg;
    int i;

    for (i = 0; i < 1; i++) {
        ticket_lock_lock(&lock);
        atomic_store_explicit(__FILE__,__LINE__,&sd1,20,memory_order_relaxed);
        int b = atomic_load_explicit(__FILE__,__LINE__,&sd1, memory_order_relaxed);
        MODEL_ASSERT(b==20);
        ticket_lock_unlock(&lock);
    }

}

int user_main(int argc, char **argv) {
    // ticket_lock_t lock;
    thrd_t t1 ,t2;
    // int i;
    atomic_init(&lock.next_ticket, 0);
    atomic_init(&lock.now_serving, 0);
    // ticket_lock_init(&lock);

    // for (i = 0; i < 2; i++) {
    //     thrd_create(&threads[i], (thrd_start_t)ThreadFunc, &lock);
    // }
    thrd_create(&t1, (thrd_start_t)&tf1, NULL);
    thrd_create(&t2, (thrd_start_t)&tf2, NULL);
    // for (i = 0; i < 2; i++) {
    //     thrd_join(threads[i]);
    // }
    thrd_join(t1);
    thrd_join(t2);
    return 0;
}
