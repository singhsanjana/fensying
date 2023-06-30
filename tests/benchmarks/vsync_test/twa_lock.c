#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define kMaxThreads 2
#define LOOP 1
#define tfLOOP 1

typedef struct {
  atomic_int flags[kMaxThreads];
  atomic_int tickets[kMaxThreads];
  atomic_int turn;
} TWALock;

void TWALock_init(TWALock* lock) {
  for (int i = 0; i < kMaxThreads; i++) {
    atomic_init(&lock->flags[i], 0);
    atomic_init(&lock->tickets[i], 0);
  }
  atomic_init(&lock->turn, 0);
}

int TWALock_lock(TWALock* lock, int id) {
  int my_ticket = atomic_fetch_add_explicit(__FILE__,__LINE__, &lock->tickets[id], 1, memory_order_seq_cst);
  atomic_store_explicit(__FILE__,__LINE__, &lock->flags[id], 1, memory_order_seq_cst);
  
  int max_ticket = atomic_load_explicit(__FILE__,__LINE__, &lock->turn, memory_order_seq_cst);
  for (int i = 0; i < LOOP; i++) {
    if (my_ticket == max_ticket) {
      return 1;
    }

    max_ticket = atomic_load_explicit(__FILE__,__LINE__, &lock->turn, memory_order_seq_cst);
  }

  return 0;
}

void TWALock_unlock(TWALock* lock, int id) {
  atomic_store_explicit(__FILE__,__LINE__, &lock->flags[id], 0, memory_order_seq_cst);
  atomic_store_explicit(__FILE__,__LINE__, &lock->turn, id + 1, memory_order_seq_cst);
}

TWALock lock;
atomic_int sd;

void thread_func(void* arg) {
  int thread_id = *(int*) arg;

  for (int i=0; i<tfLOOP; i++){
    if (TWALock_lock(&lock, thread_id)) {
      // Critical section
      atomic_store_explicit(__FILE__,__LINE__, &sd, thread_id+1, memory_order_seq_cst);
      int a = atomic_load_explicit(__FILE__,__LINE__, &sd, memory_order_seq_cst);
      MODEL_ASSERT(a == thread_id+1);

      TWALock_unlock(&lock, thread_id);
    }
  }
}

int user_main(int argc, char **argv) {
  thrd_t threads[kMaxThreads];
  int thread_ids[kMaxThreads];

  TWALock_init(&lock);
  atomic_init(&sd, 0);

  for (int i = 0; i < kMaxThreads; i++) {
    thread_ids[i] = i;
    thrd_create(&threads[i], thread_func, &thread_ids[i]);
  }

  for (int i = 0; i < kMaxThreads; i++) {
    thrd_join(threads[i]);
  }

  return 0;
}
