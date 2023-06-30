#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define K_MAX_THREADS 2
#define LOOP 3
typedef struct {
  atomic_int flags[K_MAX_THREADS];
} ArrayQ;

ArrayQ lock;

atomic_int sd;

void ArrayQ_Init(ArrayQ* array_q) {
  for (int i = 0; i < K_MAX_THREADS; i++) {
    atomic_init(&array_q->flags[i], 0);
  }
}

int ArrayQ_Lock(ArrayQ* array_q, int id) {
  for (int i = 1; i < K_MAX_THREADS; i++) {
    atomic_store_explicit(__FILE__,__LINE__,&array_q->flags[id],i, memory_order_relaxed);
    int k = (id - i + K_MAX_THREADS) % K_MAX_THREADS;
    for (int j=0;j< LOOP ;j++) {
      /* busy wait */
      if (atomic_load_explicit(__FILE__,__LINE__,&array_q->flags[k],memory_order_relaxed) < i) {
        return 1;
        // break;
      }
    }
  }

  return 0;
}

void ArrayQ_Unlock(ArrayQ* array_q, int id) {
  atomic_store_explicit(__FILE__,__LINE__,&array_q->flags[id], 0,memory_order_seq_cst);
}

void ThreadFunc(void* arg) {
  int thread_id = *(int*)arg;
  for(int i=0;i<2;i++){
    if (ArrayQ_Lock(&lock, thread_id)) {  
      // critical section
      atomic_store_explicit(__FILE__, __LINE__, &sd, thread_id+1, memory_order_relaxed);
      int a = atomic_load_explicit(__FILE__, __LINE__, &sd, memory_order_relaxed);
      MODEL_ASSERT(a == thread_id+1);
      ArrayQ_Unlock(&lock, thread_id);
    }
  }
  // return NULL;
}

int user_main(int argc, char **argv) {
  thrd_t threads[K_MAX_THREADS];
  int thread_ids[K_MAX_THREADS];

  ArrayQ_Init(&lock);
  atomic_init(&sd, 0);

  for (int i = 0; i < K_MAX_THREADS; i++) {
    thread_ids[i] = i;
    thrd_create(&threads[i], (thrd_start_t)ThreadFunc, (void*)&thread_ids[i]);
  }
    
  for (int i = 0; i < K_MAX_THREADS; i++) {
    thrd_join(threads[i]);
  }

  return 0;
}
