// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/safe155.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_0_r1_1; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r4_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v17 = (v2_r1 == 1);
  atomic_store_explicit(&atom_0_r1_1, v17, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;
  int v8_r4 = atomic_load_explicit(&vars[2+v5_r3], memory_order_acquire);
  int v18 = (v4_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v18, memory_order_release);
  int v19 = (v8_r4 == 0);
  atomic_store_explicit(&atom_1_r4_0, v19, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[3], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(&vars[3], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atomic_load_explicit(&vars[3], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(&atom_0_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(&atom_1_r4_0, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  int v16_conj = v10 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
