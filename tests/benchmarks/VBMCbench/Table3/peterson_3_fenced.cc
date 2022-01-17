#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#define LOOP 1

atomic_int level0;
atomic_int level1;
atomic_int level2;
atomic_int waiting0;
atomic_int waiting1;
atomic_int _cc_x;
atomic_int __fence_var;
void t0(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level0, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 0) || (((atomic_load_explicit(&level1, memory_order_acquire) < 0) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level0, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 0) || (((atomic_load_explicit(&level1, memory_order_acquire) < 1) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 0, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 0);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level0, 0, memory_order_release);
        }
}
void t1(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level1, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 1) || (((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 1) || (((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 1);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level1, 0, memory_order_release);
        }
}
void t2(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level2, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 2) || (((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level2, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 2) || (((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 2);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level2, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&__fence_var, 0);
atomic_init(&_cc_x, 0);
atomic_init(&waiting1, 0);
atomic_init(&waiting0, 0);
atomic_init(&level2, 0);
atomic_init(&level1, 0);
atomic_init(&level0, 0);
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

