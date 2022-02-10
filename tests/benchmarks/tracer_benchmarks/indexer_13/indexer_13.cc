/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://svn.sosy-lab.org/software/sv-benchmarks/trunk/c/pthread/indexer_true-unreach-call.c */

#include <atomic>
#include <threads.h>
#include <mutex>
#include "model-assert.h"

#include "librace.h"

#define SIZE  128
#define MAX   4
#define NUM_THREADS  13 

#define LOOP 20

std::atomic<int> table[SIZE];
std::mutex  *cas_mutex[SIZE];

thrd_t  tids[NUM_THREADS];



int cas(std::atomic<int> * tab, int h, int val, int new_val)
{
  int ret_val = 0;
  
  cas_mutex[h]->lock(); 
  if (tab[h].load(__FILE__, __LINE__, std::memory_order_acquire) == val ) {
    tab[h].store(__FILE__, __LINE__, new_val,std::memory_order_release);
    ret_val = 1;
  }
  cas_mutex[h]->unlock();

  
  return ret_val;
}


void thread_routine(void * arg)
{
  int tid;
  int m = 0, w, h;
  tid = *((int *)arg);
  

  for (int i=0; i<LOOP; i++) {
    if ( m < MAX ){
      w = (++m) * 11 + tid;
    }
    else{
      return ;
    }
    
    h = (w * 7) % SIZE;
	
    if (h<0) 
      MODEL_ASSERT(0);
	
    while ( cas(table, h, 0, w) == 0){
      h = (h+1) % SIZE;
    }

  }
  
}


int user_main(int argc, char **argv)
{
  int i, arg[NUM_THREADS];

  for (i = 0; i < SIZE; i++)
    cas_mutex[i] = new std::mutex();

  for (i = 0; i < SIZE; i++)
    atomic_init(&table[i], 0);
  
  for (i = 0; i < NUM_THREADS; i++){
    arg[i]=i;
    thrd_create(&tids[i], thread_routine, &arg[i]);
  }

  for (i = 0; i < NUM_THREADS; i++){
    thrd_join(tids[i]);
  } 

  return 0;
}


