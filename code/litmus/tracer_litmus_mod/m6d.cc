// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/m6d.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r2_1; 
atomic_int atom_2_r2_2; 
atomic_int atom_2_r1_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r9 = v2_r2 ^ v2_r2;
  atomic_store_explicit(__LINE__, &vars[1+v3_r9], 1, memory_order_release);
  int v18 = (v2_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r2_1, v18, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v5_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v6_r9 = v5_r2 ^ v5_r2;
  int v9_r1 = atomic_load_explicit(__LINE__, &vars[0+v6_r9], memory_order_acquire);
  int v19 = (v5_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r2_2, v19, memory_order_release);
  int v20 = (v9_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r1_0, v20, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&atom_2_r2_2, 0); 
  atomic_init(&atom_2_r1_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v10 = atomic_load_explicit(__LINE__, &atom_1_r2_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_2_r2_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_2_r1_0, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15_conj = v12 & v14;
  int v16_conj = v11 & v15_conj;
  int v17_conj = v10 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
