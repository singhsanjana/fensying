// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S+PPO513.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 

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
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r6 = v6_r5 ^ v6_r5;
  int v10_r7 = atomic_load_explicit(__LINE__, &vars[2+v7_r6], memory_order_acquire);
  int v12_r9 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v13_r10 = v12_r9 ^ v12_r9;
  int v14_r10 = v13_r10 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v14_r10, memory_order_release);
  int v22 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v15 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v16 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
