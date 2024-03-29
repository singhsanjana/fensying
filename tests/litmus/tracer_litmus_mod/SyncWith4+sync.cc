// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/SyncWith4+sync.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[6]; 
atomic_int atom_0_r1_1; 
atomic_int atom_0_r4_0; 
atomic_int atom_0_r6_1; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r4_0; 
atomic_int atom_2_r6_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v3_cmpeq = (v2_r6 == v2_r6);
  if (v3_cmpeq)  goto lbl_L0; else goto lbl_L0;
lbl_L0:;
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v5_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v7_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v26 = (v5_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_1, v26, memory_order_release);
  int v27 = (v7_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r4_0, v27, memory_order_release);
  int v28 = (v2_r6 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r6_1, v28, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[4], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v9_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[4], memory_order_acquire);
  int v10_cmpeq = (v9_r6 == v9_r6);
  if (v10_cmpeq)  goto lbl_L2; else goto lbl_L2;
lbl_L2:;
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v14_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[5], memory_order_acquire);
  int v29 = (v12_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v29, memory_order_release);
  int v30 = (v14_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r4_0, v30, memory_order_release);
  int v31 = (v9_r6 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r6_1, v31, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[5], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[5], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r4_0, 0); 
  atomic_init(&atom_0_r6_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&atom_2_r6_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r4_0, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r6_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r4_0, memory_order_acquire);
  int v20 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r6_1, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22_conj = v18 & v21_conj;
  int v23_conj = v17 & v22_conj;
  int v24_conj = v16 & v23_conj;
  int v25_conj = v15 & v24_conj;
  if (v25_conj == 1) MODEL_ASSERT(0);
  return 0;
}
