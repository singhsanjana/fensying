// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/LB+PPO0488.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_0_r1_1; 
atomic_int atom_1_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], v4_r3, memory_order_release);
  int v19 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_1, v19, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v7_cmpeq = (v6_r1 == v6_r1);
  if (v7_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v9_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v10_r6 = v9_r5 ^ v9_r5;
  int v13_r7 = atomic_load_explicit(__FILE__, __LINE__, &vars[3+v10_r6], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], 1, memory_order_release);
  int v15_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v20 = (v6_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
