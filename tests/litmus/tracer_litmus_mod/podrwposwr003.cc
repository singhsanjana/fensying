// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/podrwposwr003.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[6]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r7_0; 
atomic_int atom_3_r1_1; 
atomic_int atom_3_r8_1; 

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
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v4_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  int v8_r7 = atomic_load_explicit(__FILE__, __LINE__, &vars[3+v5_r6], memory_order_acquire);
  int v27 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v27, memory_order_release);
  int v28 = (v8_r7 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r7_0, v28, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[4], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v10_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[4], memory_order_acquire);
  int v11_r3 = v10_r1 ^ v10_r1;
  int v14_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[5+v11_r3], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v16_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v29 = (v10_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r1_1, v29, memory_order_release);
  int v30 = (v16_r8 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r8_1, v30, memory_order_release);

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
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&atom_3_r1_1, 0); 
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

  int v17 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r7_0, memory_order_acquire);
  int v21 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r1_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r8_1, memory_order_acquire);
  int v23_conj = v21 & v22;
  int v24_conj = v20 & v23_conj;
  int v25_conj = v19 & v24_conj;
  int v26_conj = v18 & v25_conj;
  if (v26_conj == 1) MODEL_ASSERT(0);
  return 0;
}
