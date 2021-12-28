// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0626.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r1_1; 
atomic_int atom_2_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_store_explicit(&vars[0], 3, memory_order_release);
  int v2_r4 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v3_r5 = v2_r4 ^ v2_r4;
  atomic_store_explicit(&vars[1+v3_r5], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v17 = (v5_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v17, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v7_r3 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v18 = (v7_r3 == 0);
  atomic_store_explicit(&atom_2_r3_0, v18, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v8 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v11 = (v10 == 3);
  int v12 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(&atom_2_r3_0, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
