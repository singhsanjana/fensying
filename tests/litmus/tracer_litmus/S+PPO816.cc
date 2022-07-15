// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S+PPO816.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 

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
  int v3_r3 = v2_r1 ^ v2_r1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2+v3_r3], 1, memory_order_release);
  int v5_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v6_r7 = v5_r6 ^ v5_r6;
  int v7_r7 = v6_r7 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], v7_r7, memory_order_release);
  int v9_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v11_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v12_cmpeq = (v11_r10 == v11_r10);
  if (v12_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v17 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v17, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v13 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v16_conj = v14 & v15;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
