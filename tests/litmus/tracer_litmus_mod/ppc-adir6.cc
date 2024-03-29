// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-adir6.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r1_1; 
atomic_int atom_0_r2_2; 
atomic_int atom_1_r3_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);
  int v17 = (1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_1, v17, memory_order_release);
  int v18 = (v2_r1 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r2_2, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r3 = v4_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], v5_r3, memory_order_release);
  int v19 = (v5_r3 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r3_2, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v6 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_1, memory_order_acquire);
  int v7 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r2_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r3_2, memory_order_acquire);
  int v9 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v10 = (v9 == 1);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13_conj = v10 & v12;
  int v14_conj = v8 & v13_conj;
  int v15_conj = v7 & v14_conj;
  int v16_conj = v6 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
