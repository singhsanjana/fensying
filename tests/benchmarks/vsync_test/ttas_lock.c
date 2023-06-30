#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <threads.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 1
#define INNER_LOOP 1
#define tfLOOP 1
const int kNumThreads = 2;

typedef struct {
  atomic_int lock;
} TTASLock;

void TTASLock_Init(TTASLock* lock) {
  atomic_init(&lock->lock, 0);
}

int TTASLock_Lock(TTASLock* lock) {
  int expected = 0;
  for (int i=0;i<LOOP;i++) {
    
    if(atomic_compare_exchange_strong_explicit(__FILE__,__LINE__, &lock->lock, &expected, 1, memory_order_relaxed, memory_order_relaxed)) {
      return 1;
    }

    for (int j=0;j<INNER_LOOP;j++) {
      if(!atomic_load_explicit(__FILE__,__LINE__, &lock->lock, memory_order_relaxed)) {
        return 1;
      }
    }
    expected = 0;
  }

  return 0;
}

void TTASLock_Unlock(TTASLock* lock) {
  atomic_store_explicit(__FILE__,__LINE__, &lock->lock, 0, memory_order_relaxed);
}

TTASLock lock;
atomic_int sd1;

void *tf1(void* arg) {
  for(int i=0; i<tfLOOP; i++){
    if (TTASLock_Lock(&lock)) {
      atomic_store_explicit(__FILE__,__LINE__, &sd1, 10, memory_order_seq_cst);
      int a = atomic_load_explicit(__FILE__,__LINE__, &sd1, memory_order_seq_cst);
      MODEL_ASSERT(a==10);
      TTASLock_Unlock(&lock);
    }
  }
  return NULL;
}

void *tf2(void* arg) {
  for(int i=0; i<tfLOOP; i++){
    if (TTASLock_Lock(&lock)) {
      atomic_store_explicit(__FILE__,__LINE__, &sd1, 20, memory_order_seq_cst);
      int b = atomic_load_explicit(__FILE__,__LINE__, &sd1, memory_order_seq_cst);
      MODEL_ASSERT(b==20);
      // printf("Locked by thread %ld\n", thrd_current());
      TTASLock_Unlock(&lock);
    }
  }
  return NULL;
}

int user_main(int argc, char **argv) {
  thrd_t threads[kNumThreads];

  TTASLock_Init(&lock);
  atomic_init(&sd1, 0);

  // for (int i = 0; i < kNumThreads; i++) {
  //   thrd_create(&threads[i], (thrd_start_t)ThreadFunc, NULL);
  // }
  thrd_create(&threads[0], (thrd_start_t)tf1, NULL);
  thrd_create(&threads[1], (thrd_start_t)tf2, NULL);

  // for (int i = 0; i < kNumThreads; i++) {
  thrd_join(threads[0]);
  thrd_join(threads[1]);
  // }

  return 0;
}
