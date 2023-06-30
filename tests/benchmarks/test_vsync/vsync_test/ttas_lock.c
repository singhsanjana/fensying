#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <threads.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 1
#define INNER_LOOP 1
const int kNumThreads = 2;

typedef struct {
  atomic_int lock;
} TTASLock;

void TTASLock_Init(TTASLock* lock) {
//   lock->lock = 0;
  atomic_init(&lock->lock, 0);
}

void TTASLock_Lock(TTASLock* lock) {
  int expected = 0;
  for (int i=0;i<LOOP;i++) {
    if(atomic_compare_exchange_strong(&lock->lock,&expected,1)){
      break;
    }
    for (int j=0;j<INNER_LOOP;j++)
    {
      if(!atomic_load(&lock->lock)){
        break;
      }else{
        thrd_yield();
      }
      // thrd_yield();
    }
    expected = 0;
  }
}

void TTASLock_Unlock(TTASLock* lock) {
  atomic_store(&lock->lock, 0);
}

TTASLock lock;
atomic_int sd1;
void *tf1(void* arg) {
  for(int i=0;i<2;i++){
    TTASLock_Lock(&lock);
    atomic_store_explicit(&sd1, 10, memory_order_relaxed);
    int a = atomic_load_explicit(&sd1,memory_order_relaxed);
    MODEL_ASSERT(a==10);
    // printf("Locked by thread %ld\n", thrd_current());
    TTASLock_Unlock(&lock);
  }
  return NULL;
}

void *tf2(void* arg) {
  for(int i=0;i<2;i++){
    TTASLock_Lock(&lock);
    atomic_store_explicit(&sd1, 20, memory_order_relaxed);
    int b = atomic_load_explicit(&sd1,memory_order_relaxed);
    MODEL_ASSERT(b==20);
    // printf("Locked by thread %ld\n", thrd_current());
    TTASLock_Unlock(&lock);
  }
  return NULL;
}

int user_main(int argc, char **argv) {
  thrd_t threads[kNumThreads];

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
