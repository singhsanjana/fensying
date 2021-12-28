// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0097.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r3_2; 
atomic_int atom_2_r1_2; 
atomic_int atom_3_r1_4; 
atomic_int atom_3_r4_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[1], 4, memory_order_release);
  int v25 = (v2_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r3_0, v25, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v26 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v26, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v27 = (v6_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v27, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_r3 = v8_r1 ^ v8_r1;
  int v12_r4 = atomic_load_explicit(__LINE__, &vars[0+v9_r3], memory_order_acquire);
  int v28 = (v8_r1 == 4);
  atomic_store_explicit(__LINE__, &atom_3_r1_4, v28, memory_order_release);
  int v29 = (v12_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r4_0, v29, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&atom_3_r1_4, 0); 
  atomic_init(&atom_3_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v13 = atomic_load_explicit(__LINE__, &atom_0_r3_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v16 = (v15 == 4);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_3_r1_4, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_3_r4_0, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23_conj = v14 & v22_conj;
  int v24_conj = v13 & v23_conj;
  if (v24_conj == 1) MODEL_ASSERT(0);
  return 0;
}
