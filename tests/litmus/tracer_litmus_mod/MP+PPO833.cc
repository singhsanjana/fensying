// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO833.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r12_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2+v5_r4], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 2, memory_order_release);
  int v7_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v8_r9 = v7_r8 ^ v7_r8;
  int v11_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v8_r9], memory_order_acquire);
  int v13_r12 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v20, memory_order_release);
  int v21 = (v13_r12 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r12_0, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r12_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v14 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r12_0, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
