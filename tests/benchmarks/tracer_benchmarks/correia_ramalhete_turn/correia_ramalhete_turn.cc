/* Implemented under the RA semantics
 * This benchmark is part of TRACER
 * Need to use fences under RA to avoid 
 * a violation of mutual execlusion property
 */

// Correia and Ramalhete CRTurn, Mutual Exclusion - Two linear wait software solutions
// https://github.com/pramalhe/ConcurrencyFreaks/tree/master/papers/cralgorithm-2015.pdf
//
// Shared words      = N+1
// Number of states  = 3
// Starvation-Free   = yes, with N
// Minimum SC stores = 1 + 1
// Minimum SC loads  = N+2

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 2		
#define N 3


enum State { UNLOCKED, WAITING, LOCKED };

#define CACHE_LINE  64
#define PADRATIO    (CACHE_LINE/sizeof(atomic_int))


// shared variables
atomic_int states[N*PADRATIO];
atomic_int turn;
atomic_int var;

//atomic_int __fence_var;

inline static int validate_left(int id, int lturn) {
    int i;
    if (lturn > id) {
        for (i = lturn; i < N; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) != UNLOCKED) return 0;
        }
        for (i = 0; i < id; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) != UNLOCKED) return 0;
        }
    } else {
        for (i = lturn; i < id; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) != UNLOCKED) return 0;
        }
    }
    return 1;
}

inline static int validate_right(int id, int lturn) {
    int i;
    if (lturn <= id) {
        for (i = id + 1; i < N; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) == LOCKED) return 0;
        }
        for (i = 0; i < lturn; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) == LOCKED) return 0;
        }
    } else {
        for (i = id + 1; i < lturn; i++) {
            if (atomic_load_explicit(__FILE__, __LINE__, &states[i*PADRATIO], memory_order_acquire) == LOCKED) return 0;
        }
    }
    return 1;
}

void Worker(void *arg)
{
  	int id = *((int *)arg);
	int ok;

    atomic_store_explicit(__FILE__, __LINE__, &states[id*PADRATIO], LOCKED,  memory_order_release);

   	//atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    for (int j=0; j<LOOP; j++) {
        ok = 0;
        int lturn = atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire);
        if (validate_left(id, lturn)==0) {
            atomic_store_explicit(__FILE__, __LINE__, &states[id*PADRATIO], WAITING, memory_order_release);
            for (int jj=0; jj<LOOP; jj++) {
                if (validate_left(id, lturn)==1 && lturn == atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire)) {
   					ok = 1;
                	break;
                }
                lturn = atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire);
            }
            if (ok==0) return;
            atomic_store_explicit(__FILE__, __LINE__, &states[id*PADRATIO], LOCKED, memory_order_release);
            continue;
        }

       	//atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

        ok = 0;
        for (int jj=0; jj<LOOP; jj++) {
        	if (lturn != atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire) ||
        		validate_right(id, lturn)==1) {
        		ok = 1;
        		break; 
        	}
        }

        if (ok==0) return;
		ok = 0;
        if (lturn == atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire)) 
		{
			ok = 1;	
			break;
		}
    }

	if (ok==1) {
		// critical section
		atomic_store_explicit(__FILE__, __LINE__, &var, id, memory_order_release);
		MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire) == id);

		int lturn = (atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire)+1) % N;
		atomic_store_explicit(__FILE__, __LINE__, &turn, lturn, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &states[id*PADRATIO], UNLOCKED, memory_order_release); // exit protocol
	}
	
	return;

} // Worker


int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];

	for (int i=0; i<N*PADRATIO; i++) {
	  	atomic_init(&states[i], UNLOCKED);
	}
	atomic_init(&turn, 0);
	atomic_init(&var, 0);
	//atomic_init(&__fence_var, 0);
	  
	for (int i=0; i<N; i++) {
	  	arg[i] = i;
	  	thrd_create(&ts[i], Worker, &arg[i]);
	}
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
  	return 0;
}
