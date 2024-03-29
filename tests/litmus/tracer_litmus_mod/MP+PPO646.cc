// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO646.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r9_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v3_cmpeq = (v2_r1 == v2_r1);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 2, memory_order_release);
  int v5_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v6_r7 = v5_r6 ^ v5_r6;
  int v7_r7 = v6_r7 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], v7_r7, memory_order_release);
  int v9_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v19 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v19, memory_order_release);
  int v20 = (v9_r9 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r9_1, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r9_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v10 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r9_1, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v11 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
