// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/CO-MP.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[1]; 
atomic_int atom_1_r0_2; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r1_0; 
atomic_int atom_1_r0_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r0 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v4_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v16 = (v2_r0 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r0_2, v16, memory_order_release);
  int v17 = (v4_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v17, memory_order_release);
  int v18 = (v4_r1 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_0, v18, memory_order_release);
  int v19 = (v2_r0 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r0_1, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r0_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r0_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r0_2, memory_order_acquire);
  int v6 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v7 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r0_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_0, memory_order_acquire);
  int v9 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r0_1, memory_order_acquire);
  int v10 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_0, memory_order_acquire);
  int v11_conj = v9 & v10;
  int v12_disj = v8 | v11_conj;
  int v13_conj = v7 & v12_disj;
  int v14_disj = v6 | v13_conj;
  int v15_conj = v5 & v14_disj;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
