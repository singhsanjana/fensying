/* Implemented under the RA semantics
 * This benchmark is part of TRACER
 * Need to use fences under RA to avoid 
 * a violation of mutual execlusion property
 * Get orgiral source code from: 
 * https://github.com/pramalhe/ConcurrencyFreaks/blob/master/C11/papers/cralgorithm/HehnerC11.c
 */

// Eric C. R. Hehner and R. K. Shyamasundar, An Implementation of P and V, Information Processing Letters, 1981, 12(4),
// pp. 196-197

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 5
#define N 2

enum { MAX_TICKET = INTPTR_MAX };

// shared variables
atomic_llong ticket[N];
atomic_int var;

atomic_int __fence_var;

void Worker(void *arg)
{
  	int id = *((int *)arg);
    int ok;
  
    atomic_store_explicit(__FILE__, __LINE__, &ticket[id], 0, memory_order_release);
  
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    uint64_t max = 0;
    for (int j=0; j<N; j+=1) {
      uint64_t v = atomic_load_explicit(__FILE__, __LINE__, &ticket[j], memory_order_acquire);
      if (max < v && v != MAX_TICKET ) max = v;
    } // for
    max += 1;
  
    atomic_store_explicit(__FILE__, __LINE__, &ticket[id], max, memory_order_release);

    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  
    for (int j = 0; j<N; j+=1 ) {
      ok = 0;
      for (int jj=0; jj<LOOP; jj++) {
        if (!( atomic_load(__FILE__, __LINE__, &ticket[j]) < max ||
            ( atomic_load_explicit(__FILE__, __LINE__, &ticket[j], memory_order_acquire) == max && j < id ) )) {
          ok = 1;
          break;
        }  
      }
      if (ok==0) return;
    }
  
    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, id, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==id);

    atomic_store_explicit(__FILE__, __LINE__, &ticket[id], MAX_TICKET, memory_order_release);
	
    return;

} // Worker


int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];

  	for (int i=0; i<N; i++) {
  	  	atomic_init(&ticket[i], MAX_TICKET);
  	}
  	atomic_init(&var, 0);
  	atomic_init(&__fence_var, 0);
  	  
  	for (int i=0; i<N; i++) {
  	  	arg[i] = i;
  	  	thrd_create(&ts[i], Worker, &arg[i]);
  	}
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
  	return 0;
}

