// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0704.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r4_4; 
atomic_int atom_1_r1_2; 
atomic_int atom_2_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 4, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 3, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v15 = (v4_r4 == 4);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r4_4, v15, memory_order_release);
  int v16 = (v2_r1 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_2, v16, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 5, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v6_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v17 = (v6_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r3_0, v17, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r4_4, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v7 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v8 = (v7 == 5);
  int v9 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r4_4, memory_order_acquire);
  int v10 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r3_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  int v14_conj = v8 & v13_conj;
  if (v14_conj == 1) MODEL_ASSERT(0);
  return 0;
}
