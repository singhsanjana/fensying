// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0493.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r4_4; 
atomic_int atom_1_r5_0; 
atomic_int atom_2_r4_3; 
atomic_int atom_2_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 3, memory_order_release);
  int v4_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v6_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v20, memory_order_release);
  int v21 = (v4_r4 == 4);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r4_4, v21, memory_order_release);
  int v22 = (v6_r5 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r5_0, v22, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v10_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v23 = (v10_r4 == 3);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r4_3, v23, memory_order_release);
  int v24 = (v8_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v24, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 4, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_4, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_2_r4_3, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v11 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r4_4, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r5_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r4_3, memory_order_acquire);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  int v19_conj = v11 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}