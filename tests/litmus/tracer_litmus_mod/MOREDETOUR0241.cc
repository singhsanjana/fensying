// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0241.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r1_3; 
atomic_int atom_1_r1_1; 
atomic_int atom_2_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], v4_r3, memory_order_release);
  int v23 = (v2_r1 == 3);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_3, v23, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v7_r3], memory_order_acquire);
  int v11_r6 = v10_r4 ^ v10_r4;
  int v12_r6 = v11_r6 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], v12_r6, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 3, memory_order_release);
  int v24 = (v6_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v24, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v14_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  int v25 = (v14_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v25, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_3, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_3, memory_order_acquire);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v18 = (v17 == 3);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v16 & v20_conj;
  int v22_conj = v15 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
