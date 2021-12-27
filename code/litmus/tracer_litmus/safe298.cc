// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/safe298.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r1_2; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r3_0; 
atomic_int atom_3_r1_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v23 = (v2_r1 == 2);
  atomic_store_explicit(&atom_0_r1_2, v23, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v6_r3 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v24 = (v4_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v24, memory_order_release);
  int v25 = (v6_r3 == 0);
  atomic_store_explicit(&atom_1_r3_0, v25, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[2], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v8_r1 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v9_r3 = v8_r1 ^ v8_r1;
  atomic_store_explicit(&vars[0+v9_r3], 1, memory_order_release);
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v26 = (v8_r1 == 2);
  atomic_store_explicit(&atom_3_r1_2, v26, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v10 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(&atom_0_r1_2, memory_order_acquire);
  int v15 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(&atom_1_r3_0, memory_order_acquire);
  int v17 = atomic_load_explicit(&atom_3_r1_2, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v14 & v19_conj;
  int v21_conj = v13 & v20_conj;
  int v22_conj = v11 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
