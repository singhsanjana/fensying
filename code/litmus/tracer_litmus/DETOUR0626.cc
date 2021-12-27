// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0626.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r3_1; 
atomic_int atom_1_r5_0; 
atomic_int atom_1_r7_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v21 = (v2_r3 == 0);
  atomic_store_explicit(&atom_0_r3_0, v21, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v4_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(&vars[0+v5_r4], memory_order_acquire);
  int v10_r7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v22 = (v4_r3 == 1);
  atomic_store_explicit(&atom_1_r3_1, v22, memory_order_release);
  int v23 = (v8_r5 == 0);
  atomic_store_explicit(&atom_1_r5_0, v23, memory_order_release);
  int v24 = (v10_r7 == 1);
  atomic_store_explicit(&atom_1_r7_1, v24, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_1_r7_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v11 = atomic_load_explicit(&atom_0_r3_0, memory_order_acquire);
  int v12 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(&atom_1_r3_1, memory_order_acquire);
  int v15 = atomic_load_explicit(&atom_1_r5_0, memory_order_acquire);
  int v16 = atomic_load_explicit(&atom_1_r7_1, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  int v19_conj = v13 & v18_conj;
  int v20_conj = v11 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
