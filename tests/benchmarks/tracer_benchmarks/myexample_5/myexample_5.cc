/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Example in Fig.1 in our OOPSLA paper */

/* CDSChecker is missing traces in this benchmark */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"


#  define N 5

// shared variables
atomic_int x;


void t(void* arg) {
  int tid = *((int *)arg);
  atomic_store_explicit(__FILE__, __LINE__, &x, tid, memory_order_release);
  atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

int user_main(int argc, char **argv)
{
  thrd_t tids[N];
  int arg[N];
  
  
  for (int i = 0; i < N; ++i) {
    arg[i]=i;
    thrd_create(&tids[i], t, &arg[i]);
  }
  
  for (int i = 0; i < N; ++i) {
    thrd_join(tids[i]);
  }
  
  return 0;
}
