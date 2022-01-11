// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/bc.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r2_0; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r2_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r7 = v2_r1 ^ v2_r1;
  int v6_r2 = atomic_load_explicit(&vars[0+v3_r7], memory_order_acquire);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v20, memory_order_release);
  int v21 = (v6_r2 == 0);
  atomic_store_explicit(&atom_1_r2_0, v21, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v9_r7 = v8_r1 ^ v8_r1;
  int v12_r2 = atomic_load_explicit(&vars[0+v9_r7], memory_order_acquire);
  int v22 = (v8_r1 == 1);
  atomic_store_explicit(&atom_2_r1_1, v22, memory_order_release);
  int v23 = (v12_r2 == 0);
  atomic_store_explicit(&atom_2_r2_0, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r2_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v14 = atomic_load_explicit(&atom_1_r2_0, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16 = atomic_load_explicit(&atom_2_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(&atom_2_r2_0, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_disj = v15_conj | v18_conj;
  if (v19_disj == 1) MODEL_ASSERT(0);
  return 0;
}
