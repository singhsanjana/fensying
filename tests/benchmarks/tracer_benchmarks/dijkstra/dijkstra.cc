/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Dijkstra's critical section algorithm, implemented with fences.
 *
 * URL:
 *   https://www.eecs.yorku.ca/course_archive/2007-08/W/6117/DijMutexNotes.pdf
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 10
#define N 2

// shared variables
atomic_int interested[N];
atomic_int passed[N];
atomic_int k;

atomic_int var;

atomic_int __fence_var;

void t(void *arg)
{
	int ok = 0;
	int done, _k;
 	int tid = *((int *)arg);
  	
  	atomic_store_explicit(__FILE__, __LINE__, &interested[tid], 1, memory_order_release);

  	done = 0;

  	for (int jj=0; jj<LOOP; jj++) {
  		for (int j=0; j<LOOP; j++) {
  			_k = atomic_load_explicit(__FILE__, __LINE__, &k, memory_order_acquire); 
	    	if (_k==tid) {
	    		ok = 1;
	    		break;
	    	}
	    	if (atomic_load_explicit(__FILE__, __LINE__, &interested[_k], memory_order_acquire)==0)
	    		atomic_store_explicit(__FILE__, __LINE__, &k, tid, memory_order_release); 	  
	    }
	  
	  	if (ok==0) return;

	  	atomic_store_explicit(__FILE__, __LINE__, &passed[tid], 1, memory_order_release);
	  	atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

	  	done = 1;

	  	for (int j=0; j<N; j++) {
	  		if (j==tid) continue;
	  		if (atomic_load_explicit(__FILE__, __LINE__, &passed[j], memory_order_acquire)==1) {
	  			atomic_store_explicit(__FILE__, __LINE__, &passed[tid], 0, memory_order_release);
	  			done = 0;
	  		}
	  	}

	  	if (done==1) {
	  		ok = 1; 
	  		break;
	  	} else 
	  		ok = 0;
  	}
    
  	if (ok==0) return;

  	// critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, tid, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==tid);
  
  	atomic_store_explicit(__FILE__, __LINE__, &passed[tid], 0, memory_order_release);
  	atomic_store_explicit(__FILE__, __LINE__, &interested[tid], 0, memory_order_release);

}


int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];

	for (int i=0; i<N; i++) {
	  	atomic_init(&interested[i], 0);
	  	atomic_init(&passed[i], 0);	
	}
	atomic_init(&k, 0);
	atomic_init(&var, 0);
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
