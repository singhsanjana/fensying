/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Burn's critical section algorithm, implemented with fences.
 *
 * URL:
 *   http://www.cs.yale.edu/homes/aspnes/pinewiki/attachments/MutualExclusion/burns-lynch-1993.pdf
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 10
#define N 2

// shared variables
atomic_int flags[N];
atomic_int var;

atomic_int __fence_var;

void t(void *arg)
{
  	int tid = *((int *)arg);
  	int ok = 0;
  	int restart = 0;
  
  	for (int j=0; j<LOOP; j++) {
    	// down
    	atomic_store_explicit(__FILE__, __LINE__, &flags[tid], 0, memory_order_release);
	    for (int i=0; i<tid; i++) {
	      ok = 0;
	      for (int jj=0; jj<LOOP; jj++) {
	        if (atomic_load_explicit(__FILE__, __LINE__, &flags[i], memory_order_acquire)==0) {
	          ok = 1;
	          break;
	        }
	      }
	      if (ok==0) return;
	    }
	    // up
	    atomic_store_explicit(__FILE__, __LINE__, &flags[tid], 1, memory_order_release);
	  
	  	atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

	  	for (int i=0; i<tid; i++) {
	      ok = 0;
	      for (int jj=0; jj<LOOP; jj++) {
	        if (atomic_load_explicit(__FILE__, __LINE__, &flags[i], memory_order_acquire)==0) {
	          ok = 1;
	          break;
	        }
	      }
	      if (ok==0) {
	      	restart = 1;
	      	break;
	      };
	    }
	    if (restart==0) {
	    	ok = 1;
	    	break;
	    } else ok = 0;
  	}

  	if (ok==0) return;


  	for (int i=tid+1; i<N; i++) {
      	ok = 0;
      	for (int jj=0; jj<LOOP; jj++) {
        	if (atomic_load_explicit(__FILE__, __LINE__, &flags[i], memory_order_acquire)==0) {
          		ok = 1;
          		break;
        	}
      	}
      	if (ok==0) return;
	}
  
  	// critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, tid, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==tid);
  
  	atomic_store_explicit(__FILE__, __LINE__, &flags[tid], 0, memory_order_release);
}


int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];

	for (int i=0; i<N; i++) {
	  	atomic_init(&flags[i], 0);	
	}
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
