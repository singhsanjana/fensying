// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO503.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r15_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__LINE__, &vars[2+v3_r3], memory_order_acquire);
  int v7_r6 = v6_r4 ^ v6_r4;
  int v8_r6 = v7_r6 + 1;
  atomic_store_explicit(__LINE__, &vars[3], v8_r6, memory_order_release);
  int v10_r8 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v11_r9 = v10_r8 ^ v10_r8;
  int v14_r10 = atomic_load_explicit(__LINE__, &vars[4+v11_r9], memory_order_acquire);
  int v15_r12 = v14_r10 ^ v14_r10;
  atomic_store_explicit(__LINE__, &vars[0+v15_r12], 1, memory_order_release);
  int v17_r15 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v24, memory_order_release);
  int v25 = (v17_r15 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r15_1, v25, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r15_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v18 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v19 = (v18 == 2);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_1_r15_1, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v19 & v22_conj;
  if (v23_conj == 1) MODEL_ASSERT(0);
  return 0;
}
