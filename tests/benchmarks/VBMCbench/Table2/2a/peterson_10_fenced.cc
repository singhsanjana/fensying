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
atomic_int level5;
atomic_int level6;
atomic_int level7;
atomic_int level8;
atomic_int level9;
atomic_int waiting0;
atomic_int waiting1;
atomic_int waiting2;
atomic_int waiting3;
atomic_int waiting4;
atomic_int waiting5;
atomic_int waiting6;
atomic_int waiting7;
atomic_int waiting8;
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
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 0) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level0, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 1) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level0, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 2) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level0, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 3) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level0, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 4) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level0, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 5) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level0, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 6) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level0, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 7) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level0, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 0) || ((((((((((atomic_load_explicit(&level1, memory_order_acquire) < 8) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
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
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level1, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level1, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level1, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level1, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level1, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level1, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level1, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 1) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
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
atomic_store_explicit(&waiting0, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level2, 1, memory_order_release);
atomic_store_explicit(&waiting1, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level2, 2, memory_order_release);
atomic_store_explicit(&waiting2, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level2, 3, memory_order_release);
atomic_store_explicit(&waiting3, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level2, 4, memory_order_release);
atomic_store_explicit(&waiting4, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level2, 5, memory_order_release);
atomic_store_explicit(&waiting5, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level2, 6, memory_order_release);
atomic_store_explicit(&waiting6, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level2, 7, memory_order_release);
atomic_store_explicit(&waiting7, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level2, 8, memory_order_release);
atomic_store_explicit(&waiting8, 2, memory_order_release);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 2) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 2, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 2);
atomic_store_explicit(&level2, 0, memory_order_release);
        }
}
void t3(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level3, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level3, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level3, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level3, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level3, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level3, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level3, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level3, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level3, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 3) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 3, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 3);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level3, 0, memory_order_release);
        }
}
void t4(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level4, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level4, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level4, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level4, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level4, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level4, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level4, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level4, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level4, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 4) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 4, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 4);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level4, 0, memory_order_release);
        }
}
void t5(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level5, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level5, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level5, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level5, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level5, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level5, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level5, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level5, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level5, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 5) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 5, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 5);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level5, 0, memory_order_release);
        }
}
void t6(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level6, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level6, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level6, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level6, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level6, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level6, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level6, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level6, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level6, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 6) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 6, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 6);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level6, 0, memory_order_release);
        }
}
void t7(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level7, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level7, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level7, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level7, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level7, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level7, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level7, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level7, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level7, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 7) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 7, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 7);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level7, 0, memory_order_release);
        }
}
void t8(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level8, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level9, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level8, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level9, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level8, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level9, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level8, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level9, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level8, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level9, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level8, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level9, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level8, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level9, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level8, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level9, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level8, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 8) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level9, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 8, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 8);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level8, 0, memory_order_release);
        }
}
void t9(void *)
{
for(int l=0;l<LOOP;l++)
        {
atomic_store_explicit(&level9, 0, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting0, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting0, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 0) && (atomic_load_explicit(&level1, memory_order_acquire) < 0)) && (atomic_load_explicit(&level2, memory_order_acquire) < 0)) && (atomic_load_explicit(&level3, memory_order_acquire) < 0)) && (atomic_load_explicit(&level4, memory_order_acquire) < 0)) && (atomic_load_explicit(&level5, memory_order_acquire) < 0)) && (atomic_load_explicit(&level6, memory_order_acquire) < 0)) && (atomic_load_explicit(&level7, memory_order_acquire) < 0)) && (atomic_load_explicit(&level8, memory_order_acquire) < 0)) && 1)))return;
;
atomic_store_explicit(&level9, 1, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting1, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting1, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 1) && (atomic_load_explicit(&level1, memory_order_acquire) < 1)) && (atomic_load_explicit(&level2, memory_order_acquire) < 1)) && (atomic_load_explicit(&level3, memory_order_acquire) < 1)) && (atomic_load_explicit(&level4, memory_order_acquire) < 1)) && (atomic_load_explicit(&level5, memory_order_acquire) < 1)) && (atomic_load_explicit(&level6, memory_order_acquire) < 1)) && (atomic_load_explicit(&level7, memory_order_acquire) < 1)) && (atomic_load_explicit(&level8, memory_order_acquire) < 1)) && 1)))return;
;
atomic_store_explicit(&level9, 2, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting2, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting2, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 2) && (atomic_load_explicit(&level1, memory_order_acquire) < 2)) && (atomic_load_explicit(&level2, memory_order_acquire) < 2)) && (atomic_load_explicit(&level3, memory_order_acquire) < 2)) && (atomic_load_explicit(&level4, memory_order_acquire) < 2)) && (atomic_load_explicit(&level5, memory_order_acquire) < 2)) && (atomic_load_explicit(&level6, memory_order_acquire) < 2)) && (atomic_load_explicit(&level7, memory_order_acquire) < 2)) && (atomic_load_explicit(&level8, memory_order_acquire) < 2)) && 1)))return;
;
atomic_store_explicit(&level9, 3, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting3, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting3, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 3) && (atomic_load_explicit(&level1, memory_order_acquire) < 3)) && (atomic_load_explicit(&level2, memory_order_acquire) < 3)) && (atomic_load_explicit(&level3, memory_order_acquire) < 3)) && (atomic_load_explicit(&level4, memory_order_acquire) < 3)) && (atomic_load_explicit(&level5, memory_order_acquire) < 3)) && (atomic_load_explicit(&level6, memory_order_acquire) < 3)) && (atomic_load_explicit(&level7, memory_order_acquire) < 3)) && (atomic_load_explicit(&level8, memory_order_acquire) < 3)) && 1)))return;
;
atomic_store_explicit(&level9, 4, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting4, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting4, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 4) && (atomic_load_explicit(&level1, memory_order_acquire) < 4)) && (atomic_load_explicit(&level2, memory_order_acquire) < 4)) && (atomic_load_explicit(&level3, memory_order_acquire) < 4)) && (atomic_load_explicit(&level4, memory_order_acquire) < 4)) && (atomic_load_explicit(&level5, memory_order_acquire) < 4)) && (atomic_load_explicit(&level6, memory_order_acquire) < 4)) && (atomic_load_explicit(&level7, memory_order_acquire) < 4)) && (atomic_load_explicit(&level8, memory_order_acquire) < 4)) && 1)))return;
;
atomic_store_explicit(&level9, 5, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting5, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting5, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 5) && (atomic_load_explicit(&level1, memory_order_acquire) < 5)) && (atomic_load_explicit(&level2, memory_order_acquire) < 5)) && (atomic_load_explicit(&level3, memory_order_acquire) < 5)) && (atomic_load_explicit(&level4, memory_order_acquire) < 5)) && (atomic_load_explicit(&level5, memory_order_acquire) < 5)) && (atomic_load_explicit(&level6, memory_order_acquire) < 5)) && (atomic_load_explicit(&level7, memory_order_acquire) < 5)) && (atomic_load_explicit(&level8, memory_order_acquire) < 5)) && 1)))return;
;
atomic_store_explicit(&level9, 6, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting6, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting6, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 6) && (atomic_load_explicit(&level1, memory_order_acquire) < 6)) && (atomic_load_explicit(&level2, memory_order_acquire) < 6)) && (atomic_load_explicit(&level3, memory_order_acquire) < 6)) && (atomic_load_explicit(&level4, memory_order_acquire) < 6)) && (atomic_load_explicit(&level5, memory_order_acquire) < 6)) && (atomic_load_explicit(&level6, memory_order_acquire) < 6)) && (atomic_load_explicit(&level7, memory_order_acquire) < 6)) && (atomic_load_explicit(&level8, memory_order_acquire) < 6)) && 1)))return;
;
atomic_store_explicit(&level9, 7, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting7, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting7, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 7) && (atomic_load_explicit(&level1, memory_order_acquire) < 7)) && (atomic_load_explicit(&level2, memory_order_acquire) < 7)) && (atomic_load_explicit(&level3, memory_order_acquire) < 7)) && (atomic_load_explicit(&level4, memory_order_acquire) < 7)) && (atomic_load_explicit(&level5, memory_order_acquire) < 7)) && (atomic_load_explicit(&level6, memory_order_acquire) < 7)) && (atomic_load_explicit(&level7, memory_order_acquire) < 7)) && (atomic_load_explicit(&level8, memory_order_acquire) < 7)) && 1)))return;
;
atomic_store_explicit(&level9, 8, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&waiting8, 9, memory_order_release);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
if(!((atomic_load_explicit(&waiting8, memory_order_acquire) != 9) || ((((((((((atomic_load_explicit(&level0, memory_order_acquire) < 8) && (atomic_load_explicit(&level1, memory_order_acquire) < 8)) && (atomic_load_explicit(&level2, memory_order_acquire) < 8)) && (atomic_load_explicit(&level3, memory_order_acquire) < 8)) && (atomic_load_explicit(&level4, memory_order_acquire) < 8)) && (atomic_load_explicit(&level5, memory_order_acquire) < 8)) && (atomic_load_explicit(&level6, memory_order_acquire) < 8)) && (atomic_load_explicit(&level7, memory_order_acquire) < 8)) && (atomic_load_explicit(&level8, memory_order_acquire) < 8)) && 1)))return;
;
atomic_store_explicit(&_cc_x, 9, memory_order_release);
MODEL_ASSERT(atomic_load_explicit(&_cc_x, memory_order_acquire) == 9);
atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
atomic_store_explicit(&level9, 0, memory_order_release);
        }
}
int user_main(int argc, char **argv)
{atomic_init(&__fence_var, 0);
atomic_init(&_cc_x, 0);
atomic_init(&waiting8, 0);
atomic_init(&waiting7, 0);
atomic_init(&waiting6, 0);
atomic_init(&waiting5, 0);
atomic_init(&waiting4, 0);
atomic_init(&waiting3, 0);
atomic_init(&waiting2, 0);
atomic_init(&waiting1, 0);
atomic_init(&waiting0, 0);
atomic_init(&level9, 0);
atomic_init(&level8, 0);
atomic_init(&level7, 0);
atomic_init(&level6, 0);
atomic_init(&level5, 0);
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

