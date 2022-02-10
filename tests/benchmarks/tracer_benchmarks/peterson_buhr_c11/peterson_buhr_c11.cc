/*
 * Implemeneted under the RA semantics
 * This benchmark is part of TRACER
 * Need to use fences under RA to avoid
 * a violation of mutual execlusion property
 * Get orgiral source code from:
 * https://github.com/pramalhe/ConcurrencyFreaks/blob/master/C11/papers/cralgorithm/PetersonBuhrC11.c
 */

// Peterson-Buhr in C11 with padding

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define LOOP 10
#define N 2

enum Intent { DontWantIn, WantIn };

#define CACHE_LINE  64
#define PADRATIO    (CACHE_LINE/sizeof(atomic_int))
#define inv( c ) ((c) ^ 1)

typedef struct {
  atomic_int qarray[3*PADRATIO];
} TokenC11;

typedef struct {
  TokenC11 *ns;                                           // pointer to path node from leaf to root
  int       es;                                           // left/right opponent
} Tuple;

// shared variables
static Tuple    **states;                                 // handle N threads
static int       *levels;                                 // minimal level for binary tree
static TokenC11  *t;
atomic_int var;

atomic_int __fence_var;

static inline int binary_prologue(int id, TokenC11 *t ) {
  int other = inv( id );
  int ok;
  
  atomic_int *q[2] = { &t->qarray[0], &t->qarray[PADRATIO] };
  atomic_int *turn = &t->qarray[2*PADRATIO];
  atomic_store_explicit(__FILE__, __LINE__, q[id], WantIn, memory_order_release);
  
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

  atomic_store_explicit(__FILE__, __LINE__, turn, id, memory_order_release);                                 // RACE
  
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

  ok = 0;
  for (int i=0; i<LOOP; i++) {
    if (!( atomic_load_explicit(__FILE__, __LINE__, q[other], memory_order_acquire) != DontWantIn &&
          atomic_load_explicit(__FILE__, __LINE__, turn, memory_order_acquire) == id )) {
      ok = 1;
      break;
    }
  }
  
  if (ok==0) return 0;
  else return 1;
} // binary_prologue

static inline void binary_epilogue( int id, TokenC11 *t ) {
  atomic_int *q[2] = { &t->qarray[0], &t->qarray[PADRATIO] };
  atomic_store_explicit(__FILE__, __LINE__, q[id], DontWantIn, memory_order_release);                  // exit protocol
} // binary_epilogue



void Worker(void *arg)
{
  	int id = *((int *)arg);
  	int ok = 0;
  
    int level = levels[id];
    Tuple *state = states[id];
  
    for ( int lv = 0; lv <= level; lv += 1 ) {      // entry protocol
      if (binary_prologue( state[lv].es, state[lv].ns)==0) return;
    } // for
    
    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, id, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==id);
    
    for ( int lv = level; lv >= 0; lv -= 1 ) {  // exit protocol, retract reverse order
      binary_epilogue( state[lv].es, state[lv].ns );
    } // for
}

int user_main(int argc, char **argv)
{
  	thrd_t ts[N];
  	int arg[N];
  
    t = (TokenC11 *) calloc(N, sizeof(__typeof__(t[0])));
    states = (Tuple **) calloc(N, sizeof(__typeof__(states[0])));
    levels = (int *) calloc(N, sizeof(__typeof__(levels[0])));
  
    levels[0] =  -1;
    atomic_init(&var, 0);
    atomic_init(&__fence_var, 0);
    
    for ( int id = 0; id < N; id += 1 ) {
      atomic_store_explicit(__FILE__, __LINE__, &t[id].qarray[0], DontWantIn, memory_order_release);
      atomic_store_explicit(__FILE__, __LINE__, &t[id].qarray[PADRATIO], DontWantIn, memory_order_release);
      atomic_store_explicit(__FILE__, __LINE__, &t[id].qarray[2*PADRATIO], 0, memory_order_release);
      unsigned int start = N + id, level = log2( start );
      states[id] = (Tuple *) calloc(level, sizeof(__typeof__(states[0][0])));
      levels[id] = level - 1;
      for (unsigned int s = 0; start > 1; start >>= 1, s += 1 ) {
        states[id][s].es = start & 1;
        states[id][s].ns = &t[start >> 1];
      } // for
    } // for
    
    
    for (int i=0; i<N; i++) {
        arg[i] = i;
        thrd_create(&ts[i], Worker, &arg[i]);
    }
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
  return 0;
}
