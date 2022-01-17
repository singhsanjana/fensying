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
atomic_int flag6;
atomic_int flag7;
atomic_int flag8;
atomic_int flag9;
atomic_int __fence_var;
atomic_int _cc_x;
void t0(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag0, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag1, memory_order_acquire) < 3) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag0, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag1, memory_order_acquire) == 1) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag0, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag1, memory_order_acquire) == 4) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag0, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&_cc_x, 0, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 0);
if(!((((((((((((((((((atomic_load_explicit(&flag1, memory_order_acquire) != 3) && (atomic_load_explicit(&flag1, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
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
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag1, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag1, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag1, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(atomic_load_explicit(&flag0, memory_order_acquire) < 2))return;
;
atomic_store_explicit(&_cc_x, 1, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 1);
if(!((((((((((((((((atomic_load_explicit(&flag2, memory_order_acquire) != 3) && (atomic_load_explicit(&flag2, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag1, 0, memory_order_release);
        }
}
void t2(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag2, 1, memory_order_release);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag2, 3, memory_order_release);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag2, 2, memory_order_release);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag2, 4, memory_order_release);
if(!((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 2, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 2);
if(!((((((((((((((atomic_load_explicit(&flag3, memory_order_acquire) != 3) && (atomic_load_explicit(&flag3, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag2, 0, memory_order_release);
        }
}
void t3(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag3, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag3, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag3, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag3, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 3, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 3);
if(!((((((((((((atomic_load_explicit(&flag4, memory_order_acquire) != 3) && (atomic_load_explicit(&flag4, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag3, 0, memory_order_release);
        }
}
void t4(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag4, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag4, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag4, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag4, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 4, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 4);
if(!((((((((((atomic_load_explicit(&flag5, memory_order_acquire) != 3) && (atomic_load_explicit(&flag5, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag4, 0, memory_order_release);
        }
}
void t5(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag5, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag5, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag5, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag5, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 5, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 5);
if(!((((((((atomic_load_explicit(&flag6, memory_order_acquire) != 3) && (atomic_load_explicit(&flag6, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag5, 0, memory_order_release);
        }
}
void t6(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag6, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag6, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag6, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag6, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 6, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 6);
if(!((((((atomic_load_explicit(&flag7, memory_order_acquire) != 3) && (atomic_load_explicit(&flag7, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag6, 0, memory_order_release);
        }
}
void t7(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag7, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag7, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag7, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag7, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 7, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 7);
if(!((((atomic_load_explicit(&flag8, memory_order_acquire) != 3) && (atomic_load_explicit(&flag8, memory_order_acquire) != 2)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag7, 0, memory_order_release);
        }
}
void t8(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag8, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag9, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag8, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1) || atomic_load_explicit(&flag9, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag8, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag9, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag8, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 8, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 8);
if(!((atomic_load_explicit(&flag9, memory_order_acquire) != 3) && (atomic_load_explicit(&flag9, memory_order_acquire) != 2)))return;
;
atomic_store_explicit(&flag8, 0, memory_order_release);
        }
}
void t9(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&flag9, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 3) && (atomic_load_explicit(&flag1, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 3)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 3)) && 1))return;
;
atomic_store_explicit(&flag9, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if ((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 1) || (atomic_load_explicit(&flag1, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 1)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 1) || atomic_load_explicit(&flag8, memory_order_acquire) == 1)
                {
atomic_store_explicit(&flag9, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) == 4) || (atomic_load_explicit(&flag1, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag2, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag3, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag4, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag5, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag6, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag7, memory_order_acquire) == 4)) || (atomic_load_explicit(&flag8, memory_order_acquire) == 4)))return;
;
                }
atomic_store_explicit(&flag9, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!(((((((((atomic_load_explicit(&flag0, memory_order_acquire) < 2) && (atomic_load_explicit(&flag1, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag2, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag3, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag4, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag5, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag6, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag7, memory_order_acquire) < 2)) && (atomic_load_explicit(&flag8, memory_order_acquire) < 2)))return;
;
atomic_store_explicit(&_cc_x, 9, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 9);
atomic_store_explicit(&flag9, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&_cc_x, 0);
atomic_init(&__fence_var, 0);
atomic_init(&flag9, 0);
atomic_init(&flag8, 0);
atomic_init(&flag7, 0);
atomic_init(&flag6, 0);
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
        thrd_t  _cs_tdiff6;
        thrd_t  _cs_tdiff7;
        thrd_t  _cs_tdiff8;
        thrd_t  _cs_tdiff9;
thrd_create(&_cs_tdiff0, t0, 0);
thrd_create(&_cs_tdiff1, t1, 0);
thrd_create(&_cs_tdiff2, t2, 0);
thrd_create(&_cs_tdiff3, t3, 0);
thrd_create(&_cs_tdiff4, t4, 0);
thrd_create(&_cs_tdiff5, t5, 0);
thrd_create(&_cs_tdiff6, t6, 0);
thrd_create(&_cs_tdiff7, t7, 0);
thrd_create(&_cs_tdiff8, t8, 0);
thrd_create(&_cs_tdiff9, t9, 0);
return 0;
}
}

