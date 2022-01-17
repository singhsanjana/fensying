#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
 
atomic_int c1;
atomic_int c2;

atomic_int n1;
atomic_int n2;
atomic_int _cc_x;

atomic_int barr;
#define LOOP 2
void t0(void *)
{
  int rn1 = -1;
  int rn2 = -1;
  int rc2 = -1;
  int rx = -1;
  for(int j=0;j<LOOP;j++){
    atomic_store_explicit(&c1, 1, memory_order_release);
    rn2 = atomic_load_explicit(&n2, memory_order_acquire);
    rn1 = rn2 + 1;
    atomic_store_explicit(&n1, rn1, memory_order_release);
    atomic_store_explicit(&c1, 0, memory_order_release);
    rc2 =  atomic_load_explicit(&c2, memory_order_acquire);
    if(!(rc2 == 0))
      return;
    rn2 =  atomic_load_explicit(&n2, memory_order_acquire);
    if(!(rn2 == 0 || rn1 < rn2))
      return;
    atomic_store_explicit(&_cc_x, 0, memory_order_release);
    rx =  atomic_load_explicit(&_cc_x, memory_order_acquire);
    MODEL_ASSERT(rx<=0);
    atomic_store_explicit(&n1, 0, memory_order_release);

  }
}



void t1(void *)
{
  int rn1 = -1;
  int rn2 = -1;
  int rc1 = -1;
  int rx = -1;
  for(int j=0;j<LOOP;j++){
    atomic_store_explicit(&c2, 1, memory_order_release);
    rn1=atomic_load_explicit(&n1, memory_order_acquire);
    rn2 = rn1 + 1;
    atomic_store_explicit(&n2, rn2, memory_order_release);
    atomic_store_explicit(&c2, 0, memory_order_release);
    rc1 = atomic_load_explicit(&c1, memory_order_acquire);
    if(!(rc1 == 0))
      return;
    rn1= atomic_load_explicit(&n1, memory_order_acquire);
    if(!(rn1 == 0 || rn2 <= rn1))
      return;
    atomic_store_explicit(&_cc_x, 1, memory_order_release);
    rx = atomic_load_explicit(&_cc_x, memory_order_acquire);
    MODEL_ASSERT(rx>=1);
    atomic_store_explicit(&n2, 0, memory_order_release);
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
        atomic_init(&barr, 1);
        atomic_init(&_cc_x, 0);
        thrd_create(&_t1, t0, 0);
        thrd_create(&_t2, t1, 0);
      return 0;
}


















