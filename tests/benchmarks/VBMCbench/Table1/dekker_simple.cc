#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int __fence_var;
atomic_int x;
atomic_int y ;
atomic_int _cc_x;
#define LOOP 2

void t0(void *)
{
  int ry = -1;
  int rX  = -1;

  for(int i=0;i<LOOP;i++) {
    atomic_store_explicit(&x, 1, memory_order_release);
    ry = atomic_load_explicit(&y, memory_order_acquire);;     
    if(!(ry == 0)) return;
    atomic_store_explicit(&_cc_x, 0, memory_order_release);
    rX = atomic_load_explicit(&_cc_x, memory_order_acquire);
    MODEL_ASSERT(rX<=0);
    atomic_store_explicit(&x, 0, memory_order_release);
  }
}



void t1(void *)
{
  int rx = -1;
  int rX = -1;
  for(int i=0;i<LOOP;i++) {
    atomic_store_explicit(&y, 1, memory_order_release);
    rx = atomic_load_explicit(&x, memory_order_acquire);;
    if(!(rx == 0)) return;
    atomic_store_explicit(&_cc_x, 1, memory_order_release);
    rX = atomic_load_explicit(&_cc_x, memory_order_acquire);
    MODEL_ASSERT(rX>=1);
    atomic_store_explicit(&y, 0, memory_order_release);
  }
}

int *propertyChecking(void* arg)
{
    return 0;
}


int user_main(int argc, char **argv)
{
        thrd_t _t1, _t2;
        atomic_init(&x, 0);
        atomic_init(&y, 0);
        atomic_init(&__fence_var, 0);
        atomic_init(&_cc_x, 0);
        thrd_create(&_t1, t0, 0);
        thrd_create(&_t2, t1, 0);
    return 0;
}
