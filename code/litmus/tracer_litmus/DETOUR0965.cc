// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0965.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_1; 
atomic_int atom_0_r5_2; 
atomic_int atom_1_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(&vars[0], memory_order_acquire);
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v4_r5 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  atomic_store_explicit(&vars[1+v5_r6], 1, memory_order_release);
  int v16 = (v2_r3 == 1);
  atomic_store_explicit(&atom_0_r3_1, v16, memory_order_release);
  int v17 = (v4_r5 == 2);
  atomic_store_explicit(&atom_0_r5_2, v17, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v7_r3 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v18 = (v7_r3 == 0);
  atomic_store_explicit(&atom_1_r3_0, v18, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v8 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(&atom_0_r3_1, memory_order_acquire);
  int v11 = atomic_load_explicit(&atom_0_r5_2, memory_order_acquire);
  int v12 = atomic_load_explicit(&atom_1_r3_0, memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v9 & v14_conj;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
