/* This benchmark is from POPL18 paper */

/* Example in Fig.5 in the paper 
 * https://arxiv.org/pdf/1610.01188.pdf
 * There are 2N+1 weak traces
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"


#  define N 5

// shared variables
atomic_int x;


void t1(void* arg) {
  int tid = *((int *)arg);
  
  for (int i=0; i<N; i++)
    atomic_store_explicit(__FILE__, __LINE__, &x, tid, memory_order_release);
  
  atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

void t2(void* arg) {
  int tid = *((int *)arg);
  
  for (int i=0; i<N; i++)
    atomic_store_explicit(__FILE__, __LINE__, &x, tid, memory_order_release);
  
  atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

int user_main(int argc, char **argv)
{
  thrd_t tids[2];
  int arg[2];

  arg[0]=0;
  thrd_create(&tids[0], t1, &arg[0]);
  
  arg[1]=1;
  thrd_create(&tids[1], t2, &arg[1]);
  
  for (int i = 0; i < 2; ++i) {
    thrd_join(tids[i]);
  }
  
  return 0;
}
