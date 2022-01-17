#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#define LOOP 1

atomic_int flag0;
atomic_int flag1;
atomic_int flag2;
atomic_int __fence_var;
atomic_int _cc_x;
void t0(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag0, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((atomic_load_explicit(&flag1, memory_order_acquire) < 3) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag0, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if (atomic_load_explicit(&flag1, memory_order_acquire) == 1 || atomic_load_explicit(&flag2, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag0, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&flag1, memory_order_acquire) == 4) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag0, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&_cc_x, 0, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 0);
if(!((((atomic_load_explicit(&flag1, memory_order_acquire) != 3) && (atomic_load_explicit(&flag1, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag0, 0, memory_order_release);
        }
}
void t1(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag1, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if (atomic_load_explicit(&flag0, memory_order_acquire) == 1 || atomic_load_explicit(&flag2, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag1, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag1, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(atomic_load_explicit(&flag0, memory_order_acquire) < 2))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 1);
if(!((atomic_load_explicit(&flag2, memory_order_acquire) != 3) && (atomic_load_explicit(&flag2, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag1, 0, memory_order_release);
        }
}
void t2(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag2, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag2, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if (atomic_load_explicit(&flag0, memory_order_acquire) == 1 || atomic_load_explicit(&flag1, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag2, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag2, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 2);
atomic_store_explicit(&flag2, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&_cc_x, 0);
atomic_init(&__fence_var, 0);
atomic_init(&flag2, 0);
atomic_init(&flag1, 0);
atomic_init(&flag0, 0);
{
thrd_t  _cs_tdiff0;
        thrd_t  _cs_tdiff1;
        thrd_t  _cs_tdiff2;
thrd_create(&_cs_tdiff0, t0, 0);
thrd_create(&_cs_tdiff1, t1, 0);
thrd_create(&_cs_tdiff2, t2, 0);
return 0;
}
}

