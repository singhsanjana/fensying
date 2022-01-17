#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
 
atomic_int c1;
atomic_int c2;

atomic_int n1;
atomic_int n2;
atomic_int _cc_x;
atomic_int __fence_var;
#define LOOP 2


void t0(void *)
{
  int rn1 = -1;
  int rn2 = -1;
  int rc2 = -1;
  int rx = -1;
  for(int j=0;j<LOOP;j++){
    atomic_store_explicit(__FILE__, __LINE__, &c1, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rn2 = atomic_load_explicit(__FILE__, __LINE__, &n2, memory_order_acquire);
    rn1 = rn2 + 1;
    atomic_store_explicit(__FILE__, __LINE__, &n1, rn1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(__FILE__, __LINE__, &c1, 0, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rc2 =  atomic_load_explicit(__FILE__, __LINE__, &c2, memory_order_acquire);
    if(!(rc2 == 0))
      return;
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rn2 =  atomic_load_explicit(__FILE__, __LINE__, &n2, memory_order_acquire);
    if(!(rn2 == 0 || rn1 < rn2))
      return;
    atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 0, memory_order_release);
    rx =  atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire);
    MODEL_ASSERT(rx<=0);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(__FILE__, __LINE__, &n1, 0, memory_order_release);

  }
}



void t1(void *)
{
  int rn1 = -1;
  int rn2 = -1;
  int rc1 = -1;
  int rx = -1;
  for(int j=0;j<LOOP;j++){
    atomic_store_explicit(__FILE__, __LINE__, &c2, 1, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rn1=atomic_load_explicit(__FILE__, __LINE__, &n1, memory_order_acquire);
    rn2 = rn1 + 1;
    atomic_store_explicit(__FILE__, __LINE__, &n2, rn2, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(__FILE__, __LINE__, &c2, 0, memory_order_release);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rc1 = atomic_load_explicit(__FILE__, __LINE__, &c1, memory_order_acquire);
    if(!(rc1 == 0))
      return;
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    rn1= atomic_load_explicit(__FILE__, __LINE__, &n1, memory_order_acquire);
    if(!(rn1 == 0 || rn2 <= rn1))
      return;
    atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 1, memory_order_release);
    rx = atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire);
    MODEL_ASSERT(rx>=1);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(__FILE__, __LINE__, &n2, 0, memory_order_release);
  }
}

int *propertyChecking(void* arg)
{
    return 0;
}

int user_main(int argc, char **argv)
{
        thrd_t _t1, _t2;
        atomic_init(&c1, 0);
        atomic_init(&c2, 0);
        atomic_init(&n1, 0);
        atomic_init(&n2, 0);
        atomic_init(&__fence_var, 0);
        atomic_init(&_cc_x, 0);
        thrd_create(&_t1, t0, 0);
        thrd_create(&_t2, t1, 0);
      return 0;
}


















