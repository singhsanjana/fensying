// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0073.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_2; 
atomic_int atom_0_r4_0; 
atomic_int atom_3_r1_2; 
atomic_int atom_3_r4_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  int v22 = (v2_r3 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r3_2, v22, memory_order_release);
  int v23 = (v5_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r4_0, v23, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v7_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v8_r3 = v7_r1 ^ v7_r1;
  int v11_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v8_r3], memory_order_acquire);
  int v24 = (v7_r1 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r1_2, v24, memory_order_release);
  int v25 = (v11_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r4_0, v25, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_0_r4_0, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&atom_3_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r3_2, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r4_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r4_0, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v13 & v19_conj;
  int v21_conj = v12 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}