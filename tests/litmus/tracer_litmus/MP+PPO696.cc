// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO696.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r13_1; 

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
  int v6_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v3_r3], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v8_r7 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v9_r8 = v8_r7 ^ v8_r7;
  int v12_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[3+v9_r8], memory_order_acquire);
  int v13_cmpeq = (v12_r9 == v12_r9);
  if (v13_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v15_r13 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v22 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v22, memory_order_release);
  int v23 = (v15_r13 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r13_1, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r13_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v16 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v17 = (v16 == 2);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r13_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
