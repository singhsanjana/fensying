// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/aclwsrr002.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r1_1; 
atomic_int atom_0_r3_1; 
atomic_int atom_0_r5_0; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r3_1; 
atomic_int atom_2_r5_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[1+v5_r4], memory_order_acquire);
  int v28 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_1, v28, memory_order_release);
  int v29 = (v4_r3 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r3_1, v29, memory_order_release);
  int v30 = (v8_r5 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r5_0, v30, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v10_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v13_r4 = v12_r3 ^ v12_r3;
  int v16_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v13_r4], memory_order_acquire);
  int v31 = (v10_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v31, memory_order_release);
  int v32 = (v12_r3 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r3_1, v32, memory_order_release);
  int v33 = (v16_r5 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r5_0, v33, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_0_r5_0, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&atom_2_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r3_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r5_0, memory_order_acquire);
  int v20 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v21 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r3_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r5_0, memory_order_acquire);
  int v23_conj = v21 & v22;
  int v24_conj = v20 & v23_conj;
  int v25_conj = v19 & v24_conj;
  int v26_conj = v18 & v25_conj;
  int v27_conj = v17 & v26_conj;
  if (v27_conj == 1) MODEL_ASSERT(0);
  return 0;
}
