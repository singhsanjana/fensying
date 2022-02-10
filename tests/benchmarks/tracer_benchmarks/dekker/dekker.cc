/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Dekker's critical section algorithm, implemented with fences.
 *
 * URL:
 *   http://www.justsoftwaresolutions.co.uk/threading/
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 5
#define OUTER_LOOP 2
#define LB LOOP * OUTER_LOOP

// shared variables
atomic_int flag0;
atomic_int flag1;
atomic_int turn;

atomic_int var;

atomic_int __fence_var;

void p0(void *arg)
{
  int ok1, ok2;
  for (int jj=0; jj<OUTER_LOOP; jj++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag0, 1, memory_order_release);
    
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    
    ok1 = 0;
    for (int i = 0; i<LOOP; i++)
    {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire)) {
        if (atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire) != 0)
        {
          atomic_store_explicit(__FILE__, __LINE__, &flag0, 0, memory_order_release);
          ok2 = 0;
          for (int j=0; j<LOOP; j++)
          {
            if (atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire) == 0) {
              ok2 = 1;
              break;
            }
          }
          if (ok2 == 0) return;
          atomic_store_explicit(__FILE__, __LINE__, &flag0, 1, memory_order_release);
          atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        }
        
      } else {
        ok1 = 1;
        break;
      }
    }
    if (ok1 == 0) return;
    
    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 1, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==1);
    
    atomic_store_explicit(__FILE__, __LINE__, &turn, 1, memory_order_release);
    atomic_store_explicit(__FILE__, __LINE__, &flag0, 0, memory_order_release);
  }
  
}

void p1(void *arg)
{
  int ok1, ok2;
  
  for (int jj=0; jj<OUTER_LOOP; jj++) {
    atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    
    ok1 = 0;
    for (int i=0; i<LOOP; i++)
    {
      if (atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire)) {
        if (atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire) != 1)
        {
          atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_release);
          ok2 = 0;
          for (int j=0; j<LOOP; j++)
          {
            if (atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_acquire) == 1) {
              ok2 = 1;
              break;
            }
          }
          if (ok2==0) return;
          atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_release);
          atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        }
      } else {
        ok1 = 1;
        break;
      }
    }
    if (ok1==0) return;
    
    // critical section
    atomic_store_explicit(__FILE__, __LINE__, &var, 2, memory_order_release);
    MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &var, memory_order_acquire)==2);
    
    atomic_store_explicit(__FILE__, __LINE__, &turn, 0, memory_order_release);
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
