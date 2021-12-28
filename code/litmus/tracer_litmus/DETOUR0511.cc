// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0511.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r5_3; 
atomic_int atom_1_r7_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v14 = (v2_r3 == 0);
  atomic_store_explicit(&atom_0_r3_0, v14, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  atomic_store_explicit(&vars[1], 3, memory_order_release);
  int v4_r5 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  int v8_r7 = atomic_load_explicit(&vars[0+v5_r6], memory_order_acquire);
  int v15 = (v4_r5 == 3);
  atomic_store_explicit(&atom_1_r5_3, v15, memory_order_release);
  int v16 = (v8_r7 == 0);
  atomic_store_explicit(&atom_1_r7_0, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r5_3, 0); 
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(&atom_0_r3_0, memory_order_acquire);
  int v10 = atomic_load_explicit(&atom_1_r5_3, memory_order_acquire);
  int v11 = atomic_load_explicit(&atom_1_r7_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
