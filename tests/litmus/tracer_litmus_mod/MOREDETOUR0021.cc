// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0021.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r3_2; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r4_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v6_r6 = v5_r4 ^ v5_r4;
  int v7_r6 = v6_r6 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], v7_r6, memory_order_release);
  int v19 = (v2_r3 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r3_2, v19, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v9_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v10_r3 = v9_r1 ^ v9_r1;
  int v13_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v10_r3], memory_order_acquire);
  int v20 = (v9_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v20, memory_order_release);
  int v21 = (v13_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r4_0, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r3_2, memory_order_acquire);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r4_0, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}