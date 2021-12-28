// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-cookbook6.2.2.noloop.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r3_1; 
atomic_int atom_1_r5_0; 
atomic_int atom_2_r3_1; 
atomic_int atom_2_r5_55; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 55, memory_order_release);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r12 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r12 = v2_r12 & 0;
  int v6_r5 = atomic_load_explicit(&vars[0+v3_r12], memory_order_acquire);
  int v20 = (v2_r12 == 1);
  atomic_store_explicit(&atom_1_r3_1, v20, memory_order_release);
  int v21 = (v6_r5 == 0);
  atomic_store_explicit(&atom_1_r5_0, v21, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r12 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v9_r12 = v8_r12 & 0;
  int v12_r5 = atomic_load_explicit(&vars[0+v9_r12], memory_order_acquire);
  int v22 = (v8_r12 == 1);
  atomic_store_explicit(&atom_2_r3_1, v22, memory_order_release);
  int v23 = (v12_r5 == 55);
  atomic_store_explicit(&atom_2_r5_55, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&atom_2_r5_55, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(&atom_1_r3_1, memory_order_acquire);
  int v14 = atomic_load_explicit(&atom_1_r5_0, memory_order_acquire);
  int v15 = atomic_load_explicit(&atom_2_r3_1, memory_order_acquire);
  int v16 = atomic_load_explicit(&atom_2_r5_55, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  int v19_conj = v13 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
