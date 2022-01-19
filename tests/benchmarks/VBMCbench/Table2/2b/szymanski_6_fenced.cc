#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#define LOOP 1

atomic_int flag0;
atomic_int flag1;
atomic_int flag2;
atomic_int flag3;
atomic_int flag4;
atomic_int flag5;
atomic_int __fence_var;
atomic_int _cc_x;
void t0(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag0, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag0, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag0, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag0, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 0, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 0);
if(!((((((((((atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) != 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag0, 0, memory_order_release);
        }
}
void t1(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag1, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag1, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag1, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 2))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 1);
if(!((((((((atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) != 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_release);
        }
}
void t2(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag2, 1, memory_order_release);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag2, 3, memory_order_release);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag2, 2, memory_order_release);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag2, 4, memory_order_release);
if(!((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 2, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 2);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag2, 0, memory_order_release);
        }
}
void t3(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag3, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag3, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag3, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag3, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 3, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 3);
if(!((((atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag3, 0, memory_order_release);
        }
}
void t4(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag4, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag4, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag4, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag4, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 4, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 4);
if(!((atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag5, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag4, 0, memory_order_release);
        }
}
void t5(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(__FILE__, __LINE__, &flag5, 1, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 3) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(__FILE__, __LINE__, &flag5, 3, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if ((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 1) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 1) || atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 1)
                {
atomic_store_explicit(__FILE__, __LINE__, &flag5, 2, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) == 4) || (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(__FILE__, __LINE__, &flag5, 4, memory_order_release);
atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(__FILE__, __LINE__, &flag0, memory_order_acquire) < 2) && (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(__FILE__, __LINE__, &flag4, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(__FILE__, __LINE__, &_cc_x, 5, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &_cc_x, memory_order_acquire) == 5);
atomic_store_explicit(__FILE__, __LINE__, &flag5, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&_cc_x, 0);
atomic_init(&__fence_var, 0);
atomic_init(&flag5, 0);
atomic_init(&flag4, 0);
atomic_init(&flag3, 0);
atomic_init(&flag2, 0);
atomic_init(&flag1, 0);
atomic_init(&flag0, 0);
{
thrd_t  _cs_tdiff0;
        thrd_t  _cs_tdiff1;
        thrd_t  _cs_tdiff2;
        thrd_t  _cs_tdiff3;
        thrd_t  _cs_tdiff4;
        thrd_t  _cs_tdiff5;
thrd_create(&_cs_tdiff0, t0, 0);
thrd_create(&_cs_tdiff1, t1, 0);
thrd_create(&_cs_tdiff2, t2, 0);
thrd_create(&_cs_tdiff3, t3, 0);
thrd_create(&_cs_tdiff4, t4, 0);
thrd_create(&_cs_tdiff5, t5, 0);
return 0;
}
}

