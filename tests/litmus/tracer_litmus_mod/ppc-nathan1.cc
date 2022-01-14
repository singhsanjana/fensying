// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-nathan1.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r8_1; 
atomic_int atom_0_r9_0; 
atomic_int atom_1_r8_1; 
atomic_int atom_1_r9_0; 
atomic_int atom_3_r8_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r12 = v2_r8 ^ v2_r8;
  int v6_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[1+v3_r12], memory_order_acquire);
  int v24 = (v2_r8 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r8_1, v24, memory_order_release);
  int v25 = (v6_r9 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r9_0, v25, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);
  int v8_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v9_r12 = v8_r8 ^ v8_r8;
  int v12_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v9_r12], memory_order_acquire);
  int v26 = (v8_r8 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r8_1, v26, memory_order_release);
  int v27 = (v12_r9 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r9_0, v27, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v14_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v28 = (v14_r8 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r8_1, v28, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r8_1, 0); 
  atomic_init(&atom_0_r9_0, 0); 
  atomic_init(&atom_1_r8_1, 0); 
  atomic_init(&atom_1_r9_0, 0); 
  atomic_init(&atom_3_r8_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r8_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r9_0, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r8_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r9_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r8_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23_conj = v15 & v22_conj;
  if (v23_conj == 1) MODEL_ASSERT(0);
  return 0;
}