/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Szymaski's critical section algorithm, implemented with fences.
 *
 * Adapted from: https://svn.sosy-lab.org/software/sv-benchmarks/trunk/c/pthread-atomic/szymanski_true-unreach-call.c
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 3
#define OUTER_LOOP 2
#define LB LOOP*OUTER_LOOP

// shared variables
atomic_int flag1; 
atomic_int flag2;

atomic_int var;

atomic_int __fence_var;

void p1(void *arg)
{
  int ok;
  for (int j=0; j<OUTER_LOOP; j++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_release);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3) {
        ok = 1;
        break;	
      }	
    }
    if (ok==0) return;

    atomic_store_explicit(__FILE__, __LINE__, &flag1, 3, memory_order_release);

    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    if (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1) {
      atomic_store_explicit(__FILE__, __LINE__, &flag1, 2, memory_order_release);

      atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

      ok =0;
      for (int i=0; i<LOOP; i++) {
        if (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4) {
          ok = 1;
          break;		
        }
      }
      if (ok==0) return;
    }

    atomic_store_explicit(__FILE__, __LINE__, &flag1, 4, memory_order_release);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 2) {
        ok = 1;
        break;	
      }
    }
    if (ok==0) return;

      // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 1, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==1);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (2 > atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) || atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) > 3) {
        ok = 1;
        break;	
      }
    } 
    if (ok==0) return;

    atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_release);
  }
}

void p2(void *arg)
{
  int ok;
  for (int j=0; j<OUTER_LOOP; j++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag2, 1, memory_order_release);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3) {
        ok = 1;
        break;	
      }	
    }
    if (ok==0) return;

    atomic_store_explicit(__FILE__, __LINE__, &flag2, 3, memory_order_release);

    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

    if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1) {
      atomic_store_explicit(__FILE__, __LINE__, &flag2, 2, memory_order_release);

      atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);

      ok =0;
      for (int i=0; i<LOOP; i++) {
        if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4) {
          ok = 1;
          break;		
        }
      }
      if (ok==0) return;

    }

    atomic_store_explicit(__FILE__, __LINE__, &flag2, 4, memory_order_release);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 2) {
        ok = 1;
        break;	
      }
    }
    if (ok==0) return;

      // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 2, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==2);

    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (2 > atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) || atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) > 3) {
        ok = 1;
        break;	
      }
    } 
    if (ok==0) return;

    atomic_store_explicit(__FILE__, __LINE__, &flag2, 0, memory_order_release);
  }
}

int user_main(int argc, char **argv)
{
	thrd_t a, b;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&var, 0);
  atomic_init(&__fence_var, 0);

  thrd_create(&a, p1, NULL);
  thrd_create(&b, p2, NULL);

 return 0;
}
