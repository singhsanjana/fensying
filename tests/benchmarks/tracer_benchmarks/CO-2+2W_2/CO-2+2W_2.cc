/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

/* There are N weak traces */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define N 2

atomic_int vars[1];

void t(void *arg){
  int tid = *((int *)arg);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], tid, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], tid, memory_order_release);
}


int user_main(int argc, char **argv){
  thrd_t ts[N];
  int arg[N];
  
  atomic_init(&vars[0], 0);
  
  for (int i=0; i<N; i++) {
    arg[i]=i;
    thrd_create(&ts[i], t, &arg[i]);
  }
  
  for (int i=0; i<N; i++)
    thrd_join(ts[i]);
  
  int v1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v2 = (v1 == N);
  if (v2 == 1) MODEL_ASSERT(0);
  return 0;
}
