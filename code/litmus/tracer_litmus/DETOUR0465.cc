// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0465.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r3_0; 
atomic_int atom_0_r6_1; 
atomic_int atom_1_r1_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v4_r6 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  int v17 = (v2_r3 == 0);
  atomic_store_explicit(&atom_0_r3_0, v17, memory_order_release);
  int v18 = (v4_r6 == 1);
  atomic_store_explicit(&atom_0_r6_1, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v8_r3 = v7_r3 + 1;
  atomic_store_explicit(&vars[0], v8_r3, memory_order_release);
  int v19 = (v6_r1 == 2);
  atomic_store_explicit(&atom_1_r1_2, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_0_r6_1, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(&atom_0_r3_0, memory_order_acquire);
  int v10 = atomic_load_explicit(&atom_0_r6_1, memory_order_acquire);
  int v11 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(&atom_1_r1_2, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v10 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
