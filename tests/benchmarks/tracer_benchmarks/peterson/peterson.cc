/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Peterson's critical section algorithm, implemented with fences.
 *
 * Adapted from: https://svn.sosy-lab.org/software/sv-benchmarks/trunk/c/pthread-atomic/peterson_true-unreach-call.c
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 2
#define OUTER_LOOP 3
#define LB LOOP*OUTER_LOOP

atomic_int flag0;
atomic_int flag1;
atomic_int turn;

atomic_int var; // to avoid race

atomic_int __fence_var;

void p0(void *arg)
{
  for (int j=0; j<OUTER_LOOP; j++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag0, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    atomic_store_explicit(__FILE__, __LINE__, &turn, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    int ok = 0;
    
    for (int i=0; i<LOOP; i++) { // using LOOP version to avoid unterminated runs under CDSChecker
      int _flag1 = atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire);
      int _turn =  atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire);
      if ( _flag1 != 1 || _turn != 1) {
        ok = 1;	
        break;
      } 
    }
    if (ok==0) return;

    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 1, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==1);

    atomic_store_explicit(__FILE__, __LINE__, &flag0, 0, memory_order_release);
  }
}

void p1(void *arg)
{
  for (int j=0; j<OUTER_LOOP; j++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    atomic_store_explicit(__FILE__, __LINE__, &turn, 0, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    int ok = 0;
    for (int i=0; i<LOOP; i++) {
      int _flag0 = atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire);
      int _turn =  atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire);
      if (_flag0 != 1 || _turn != 0) {
        ok = 1;
        break;		
      } 
    }
    if (ok==0) return; 

    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 2, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==2);

    atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_release);
  }
}

int user_main(int argc, char **argv)
{
 thrd_t a, b;

 atomic_init(&flag0, 0);
 atomic_init(&flag1, 0);
 atomic_init(&turn, 0);
 atomic_init(&var, 0);
 atomic_init(&__fence_var, 0);

 thrd_create(&a, p0, NULL);
 thrd_create(&b, p1, NULL);

 thrd_join(a);
 thrd_join(b);

 return 0;
}
