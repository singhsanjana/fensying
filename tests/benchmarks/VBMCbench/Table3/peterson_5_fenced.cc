#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#define LOOP 1

atomic_int level0;
atomic_int level1;
atomic_int level2;
atomic_int level3;
atomic_int level4;
atomic_int waiting0;
atomic_int waiting1;
atomic_int waiting2;
atomic_int waiting3;
atomic_int _cc_x;
atomic_int __fence_var;
void t0(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &level0, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting0, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting0, memory_order_acquire) != 0) || (((((atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 0) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level0, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting1, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting1, memory_order_acquire) != 0) || (((((atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 1) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level0, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting2, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting2, memory_order_acquire) != 0) || (((((atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level0, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting3, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting3, memory_order_acquire) != 0) || (((((atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 0, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 0);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &level0, 0, memory_order_release);
        }
}
void t1(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &level1, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting0, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting0, memory_order_acquire) != 1) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 0) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level1, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting1, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting1, memory_order_acquire) != 1) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 1) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level1, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting2, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting2, memory_order_acquire) != 1) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level1, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting3, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting3, memory_order_acquire) != 1) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 1);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &level1, 0, memory_order_release);
        }
}
void t2(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &level2, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting0, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting0, memory_order_acquire) != 2) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 0) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level2, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting1, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting1, memory_order_acquire) != 2) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 1) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level2, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting2, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting2, memory_order_acquire) != 2) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level2, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting3, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting3, memory_order_acquire) != 2) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 2);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &level2, 0, memory_order_release);
        }
}
void t3(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &level3, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting0, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting0, memory_order_acquire) != 3) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 0) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level3, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting1, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting1, memory_order_acquire) != 3) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 1) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level3, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting2, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting2, memory_order_acquire) != 3) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level3, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting3, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting3, memory_order_acquire) != 3) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level4, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 3, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 3);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &level3, 0, memory_order_release);
        }
}
void t4(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &level4, 0, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting0, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting0, memory_order_acquire) != 4) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 0) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 0)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level4, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting1, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting1, memory_order_acquire) != 4) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 1) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 1)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level4, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting2, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting2, memory_order_acquire) != 4) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &level4, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &waiting3, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(__FILE__, __LINE__, &waiting3, memory_order_acquire) != 4) || (((((atomic_load_explicit(__FILE__, __LINE__, &level0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &level1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &level3, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 4, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 4);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &level4, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&__fence_var, 0);
atomic_init(&_cc_x, 0);
atomic_init(&waiting3, 0);
atomic_init(&waiting2, 0);
atomic_init(&waiting1, 0);
atomic_init(&waiting0, 0);
atomic_init(&level4, 0);
atomic_init(&level3, 0);
atomic_init(&level2, 0);
atomic_init(&level1, 0);
atomic_init(&level0, 0);
{
thrd_t  _cs_tdiff0;
        thrd_t  _cs_tdiff1;
        thrd_t  _cs_tdiff2;
        thrd_t  _cs_tdiff3;
        thrd_t  _cs_tdiff4;
thrd_create(&_cs_tdiff0, t0, 0);
thrd_create(&_cs_tdiff1, t1, 0);
thrd_create(&_cs_tdiff2, t2, 0);
thrd_create(&_cs_tdiff3, t3, 0);
thrd_create(&_cs_tdiff4, t4, 0);
return 0;
}
}

