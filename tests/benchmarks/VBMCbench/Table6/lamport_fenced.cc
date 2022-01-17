#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 1

atomic_int x, y;
atomic_int b1, b2;
atomic_int _cc_X;

atomic_int __fence_var;

void t0(void *)
{
  int ry1 = -1, ry2 = -1, ry3 = -1, ry4 = -1, rx = -1, rb2 = -1;
  for(int l1=0;l1<LOOP;l1++) {
    for(int l2=0;l2<LOOP;l2++) 
    {
        atomic_store_explicit(&b1, 1, memory_order_release);
        atomic_store_explicit(&x, 1, memory_order_release);
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
        ry1 = atomic_load_explicit(&y, memory_order_acquire);;
        if (ry1 != 0)
        {
            atomic_store_explicit(&b1, 0, memory_order_release);
            ry2 = atomic_load_explicit(&y, memory_order_acquire);
            int l3 = 0;
            while (ry2 != 0 && l3<LOOP)
            {
                ry2 = atomic_load_explicit(&y, memory_order_acquire);;
                l3++;
            };
            if(ry2!=0)
                return;
            continue;
        }
        atomic_store_explicit(&y, 1, memory_order_release); 
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
        rx = atomic_load_explicit(&x, memory_order_acquire);;
        if (rx != 1)
        {
            atomic_store_explicit(&b1, 0, memory_order_release);
            rb2 = atomic_load_explicit(&b2, memory_order_acquire);
            int l3 =0 ;
            while (rb2 >= 1 &&l3<LOOP)
            {
                rb2 = atomic_load_explicit(&b2, memory_order_acquire);
                l3++;
            };
            if(rb2>=1)
                return;
            atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
            ry3 = atomic_load_explicit(&y, memory_order_acquire);;
            if (ry3 != 1)
            {
                ry4 = atomic_load_explicit(&y, memory_order_acquire);
                l3 = 0;
                while (ry4 != 0 && l3<LOOP)
                {
                    ry4 = atomic_load_explicit(&y, memory_order_acquire);;
                    l3++;
                };
                if(ry4!=0)
                    return;
                continue;
            }
        }
        break;
    }
    atomic_store_explicit(&_cc_X, 0, memory_order_release);
    rx = atomic_load_explicit(&_cc_X, memory_order_acquire);;
    MODEL_ASSERT(rx <= 0);
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(&y, 0, memory_order_release); 
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(&b1, 0, memory_order_release);
  }
}

void t1(void *)
{
  int ry1 = -1, ry2 = -1, ry3 = -1, ry4 = -1, rx = -1, rb1 = -1;
for(int l1=0;l1<LOOP;l1++) {
    for(int l2=0;l2<LOOP;l2++) 
    {
        atomic_store_explicit(&b2, 1, memory_order_release);
        atomic_store_explicit(&x, 2, memory_order_release);
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
        ry1 = atomic_load_explicit(&y, memory_order_acquire);;
        if (ry1 != 0)
        {
            atomic_store_explicit(&b2, 0, memory_order_release);
            ry2 = atomic_load_explicit(&y, memory_order_acquire);
            int l3 =0;
            while (ry2 != 0 && l3 <LOOP)
            {
                ry2 = atomic_load_explicit(&y, memory_order_acquire);
                l3++;
            };
        	if(ry2!=0)return;    
	continue;
        }
        atomic_store_explicit(&y, 2, memory_order_release); 
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
        rx = atomic_load_explicit(&x, memory_order_acquire);;
        if (rx != 2)
        {
            atomic_store_explicit(&b2, 0, memory_order_release);
            rb1 = atomic_load_explicit(&b1, memory_order_acquire);
            int l3 =0 ;
            while (rb1 >= 1 && l3<LOOP)
            {
                rb1 = atomic_load_explicit(&b1, memory_order_acquire);
                l3++;
            };
   		if(rb1>=1)return; 
	atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
            ry3 = atomic_load_explicit(&y, memory_order_acquire);;
            if (ry3 != 2)
            {
                ry4 = atomic_load_explicit(&y, memory_order_acquire);
                l3 =0 ;
                while (ry4 != 0 &&l3<LOOP)
                {
                    ry4 = atomic_load_explicit(&y, memory_order_acquire);
                    l3++;
                };
               if(ry4!=0)return; 
		continue;
            }
        }
        break;
    }
    atomic_store_explicit(&_cc_X, 1, memory_order_release);
    rx = atomic_load_explicit(&_cc_X, memory_order_acquire);;
    MODEL_ASSERT(rx >= 1);
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(&y, 0, memory_order_release); 
    atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
    atomic_store_explicit(&b2, 0, memory_order_release);
  }
}


int *propertyChecking(void* arg)
{
    return 0;
}

int user_main(int argc, char **argv)
{
        thrd_t _t1, _t2;
        atomic_init(&b1, 0);
        atomic_init(&b2, 0);
        atomic_init(&x, 0);
        atomic_init(&y, 0);
        atomic_init(&__fence_var, 0);
        atomic_init(&_cc_X, 0);
        thrd_create(&_t1, t0, 0);
        thrd_create(&_t2, t1, 0);
      return 0;
}




