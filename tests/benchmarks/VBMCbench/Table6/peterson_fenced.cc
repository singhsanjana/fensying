#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#define LOOP 1

atomic_int flag1;
atomic_int flag2;
atomic_int turn;
atomic_int _cc_x;
atomic_int __fence_var;
void t0(void *)
{
int rflag2; rflag2 = -1;
        int rturn; rturn = -1;
        int rx; rx = -1;
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&turn, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
rflag2 = atomic_load_explicit(&flag2, memory_order_acquire);
rturn = atomic_load_explicit(&turn, memory_order_acquire);
if(!((rflag2 != 1) || (rturn != 1)))return;
;
atomic_store_explicit(&_cc_x, 0, memory_order_release);
rx = atomic_load_explicit(&_cc_x, memory_order_acquire);
MODEL_ASSERT(rx <= 0);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&flag1, 0, memory_order_release);
        }
}
void t1(void *)
{
int rflag1; rflag1 = -1;
        int rturn; rturn = -1;
        int rx; rx = -1;
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag2, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&turn, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
rflag1 = atomic_load_explicit(&flag1, memory_order_acquire);
rturn = atomic_load_explicit(&turn, memory_order_acquire);
if(!((rflag1 != 1) || (rturn != 0)))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
rx = atomic_load_explicit(&_cc_x, memory_order_acquire);
MODEL_ASSERT(rx >= 1);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&flag2, 0, memory_order_release);
        }
}
int *propertyChecking(void *arg)
{
return 0;
}
int user_main(int argc, char **argv)
{atomic_init(&__fence_var, 0);
atomic_init(&_cc_x, 0);
atomic_init(&turn, 0);
atomic_init(&flag2, 0);
atomic_init(&flag1, 0);
{
thrd_t  _cs_t1;
        thrd_t  _cs_t2;
thrd_create(&_cs_t1, t0, 0);
thrd_create(&_cs_t2, t1, 0);
return 0;
}
}

