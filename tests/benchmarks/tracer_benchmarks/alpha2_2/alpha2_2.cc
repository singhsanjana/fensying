/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

/* There are N^2+N+1 weak traces */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define N 2

atomic_int vars[1];

void writer(void *arg){
  int tid = *((int *)arg);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], tid, memory_order_release);
}

void reader(void *arg){
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], N, memory_order_release);
  atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
}

int user_main(int argc, char **argv){
  thrd_t ws[N];
  thrd_t read;
  int arg[N];
  
  atomic_init(&vars[0], 1);
  
  for (int i=0; i<N; i++) {
    arg[i]=i;
    thrd_create(&ws[i], writer, &arg[i]);
  }
  thrd_create(&read, reader, NULL);
  
  for (int i=0; i<N; i++)
    thrd_join(ws[i]);
  thrd_join(read);
  
  return 0;
}
