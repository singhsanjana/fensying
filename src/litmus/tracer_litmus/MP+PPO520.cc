// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO520.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r12_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  int v4_r4 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v6_r5 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7_r6 = v6_r5 ^ v6_r5;
  int v10_r7 = atomic_load_explicit(&vars[2+v7_r6], memory_order_acquire);
  int v11_r9 = v10_r7 ^ v10_r7;
  atomic_store_explicit(&vars[0+v11_r9], 1, memory_order_release);
  int v13_r12 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v23 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v23, memory_order_release);
  int v24 = (v13_r12 == 1);
  atomic_store_explicit(&atom_1_r12_1, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r12_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v14 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v17 = (v16 == 2);
  int v18 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(&atom_1_r12_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v15 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
