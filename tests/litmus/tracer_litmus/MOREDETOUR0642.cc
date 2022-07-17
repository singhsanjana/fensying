// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0642.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r4_4; 
atomic_int atom_1_r4_3; 
atomic_int atom_1_r1_1; 
atomic_int atom_3_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 3, memory_order_release);
  int v2_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r4 == v2_r4);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);
  int v20 = (v2_r4 == 4);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r4_4, v20, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v7_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v21 = (v7_r4 == 3);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r4_3, v21, memory_order_release);
  int v22 = (v5_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v22, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 4, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v9_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v23 = (v9_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r3_0, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r4_4, 0); 
  atomic_init(&atom_1_r4_3, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_3_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v10 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r4_4, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r4_3, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r3_0, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  int v19_conj = v11 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}