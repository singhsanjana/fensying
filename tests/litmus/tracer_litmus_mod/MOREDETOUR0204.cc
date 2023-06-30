// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0204.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r1_2; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r6_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], v4_r3, memory_order_release);
  int v18 = (v2_r1 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_2, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2+v7_r3], 1, memory_order_release);
  int v9_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  int v19 = (v6_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v19, memory_order_release);
  int v20 = (v9_r6 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r6_0, v20, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r6_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v10 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r6_0, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15_conj = v12 & v14;
  int v16_conj = v11 & v15_conj;
  int v17_conj = v10 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
