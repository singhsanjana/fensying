#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

#define LOOP 1
#define tfLOOP 1
#define numThread 2

typedef struct {
    atomic_int next_ticket;
    atomic_int now_serving;
} ticket_lock_t;

ticket_lock_t lock;
atomic_int sd;

void ticket_lock_init(ticket_lock_t *lock) {
    atomic_init(&lock->next_ticket, 0);
    atomic_init(&lock->now_serving, 0);
}

static inline int ticket_lock_lock(ticket_lock_t *lock) {
    int my_ticket = atomic_fetch_add_explicit(__FILE__,__LINE__, &lock->next_ticket, 1, memory_order_relaxed);
    
    for( int i=0; i<LOOP; i++) {
        if(atomic_load_explicit(__FILE__,__LINE__, &lock->now_serving, memory_order_relaxed) == my_ticket) {
            return 1;
        }
    }

    return 0;
}

static inline void ticket_lock_unlock(ticket_lock_t *lock) {
    atomic_fetch_add_explicit(__FILE__,__LINE__,&lock->now_serving, 1, memory_order_relaxed);
}

static void tf(void *arg) {
    int cs_val = *(int*)arg;
    cs_val++;
    int i;

    for (i = 0; i < tfLOOP; i++) {
        if (ticket_lock_lock(&lock)) {
            atomic_store_explicit(__FILE__,__LINE__, &sd, cs_val, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__,__LINE__, &sd, memory_order_seq_cst);
            MODEL_ASSERT(a == cs_val);
            // ticket_lock_unlock(&lock);
        }
        ticket_lock_unlock(&lock);
    }

}

int user_main(int argc, char **argv) {
    thrd_t t[numThread];
    int thread_ids[numThread];
    
    ticket_lock_init(&lock);
    atomic_init(&sd, 0);

    for (int n = 0; n  < numThread; n++) {
        thread_ids[n] = n;
        thrd_create(&t[n], (thrd_start_t)tf, (void*)&thread_ids[n]);
    }
    
    for (int n = 0; n  < numThread; n++) {
        thrd_join(t[n]);
    }
    
    return 0;
}
