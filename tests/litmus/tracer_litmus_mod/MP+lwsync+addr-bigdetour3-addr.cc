// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+lwsync+addr-bigdetour3-addr.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r9_0; 
atomic_int atom_1_r4_0; 
atomic_int atom_1_r6_1; 
atomic_int atom_3_r1_1; 

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
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v3_r3], memory_order_acquire);
  int v8_r6 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v9_r8 = v8_r6 ^ v8_r6;
  int v12_r9 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v9_r8], memory_order_acquire);
  int v26 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v26, memory_order_release);
  int v27 = (v12_r9 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r9_0, v27, memory_order_release);
  int v28 = (v6_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r4_0, v28, memory_order_release);
  int v29 = (v8_r6 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r6_1, v29, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[4], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v14_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[4], memory_order_acquire);
  int v15_r3 = v14_r1 ^ v14_r1;
  int v16_r2 = v15_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], v16_r2, memory_order_release);
  int v30 = (v14_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r1_1, v30, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r9_0, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&atom_1_r6_1, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r9_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r4_0, memory_order_acquire);
  int v20 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r6_1, memory_order_acquire);
  int v21 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r1_1, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v19 & v22_conj;
  int v24_conj = v18 & v23_conj;
  int v25_conj = v17 & v24_conj;
  if (v25_conj == 1) MODEL_ASSERT(0);
  return 0;
}
