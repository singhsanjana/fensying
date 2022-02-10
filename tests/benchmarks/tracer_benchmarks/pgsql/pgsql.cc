/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * PostgreSQL bug under Power.
 *
 * URL: Listing 1.1 in
 *   https://arxiv.org/pdf/1207.7264.pdf
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 10
#define N 5

// shared variables
atomic_int latch[N];
atomic_int flag[N];
atomic_int var;

atomic_int __fence_var;

void t(void *arg)
{
  	int tid = *((int *)arg);
  	int ok = 0;
  	
    for (int i=0; i<LOOP; i++) {
    	if (atomic_load_explicit(__FILE__, __LINE__, &latch[tid], memory_order_acquire)==1) {
    		ok = 1;
    		break;
    	}    
    }
    
    if (ok==0) return;

	MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &latch[tid], memory_order_acquire)==0 ||
				 atomic_load_explicit(__FILE__, __LINE__, &flag[tid], memory_order_acquire)==1);

	atomic_store_explicit(__FILE__, __LINE__, &latch[tid], 0, memory_order_release);

	if (atomic_load_explicit(__FILE__, __LINE__, &flag[tid], memory_order_acquire)==1) {
		atomic_store_explicit(__FILE__, __LINE__, &flag[tid], 0, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &flag[(tid+1)%N], 1, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &latch[(tid+1)%N], 1, memory_order_release);	
	} 
  
}


int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];

	for (int i=1; i<N; i++) {
	  	atomic_init(&latch[i], 0);
	  	atomic_init(&flag[i], 0);	
	}
	atomic_init(&latch[0], 1);
	atomic_init(&flag[0], 1);
	atomic_init(&__fence_var, 0);
	  
	for (int i=0; i<N; i++) {
	  	arg[i] = i;
	  	thrd_create(&ts[i], t, &arg[i]);
	}
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
  return 0;
}
