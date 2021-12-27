// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/LB+PPO0005.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
atomic_int atom_0_r1_1; 
atomic_int atom_1_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(&vars[1], v4_r3, memory_order_release);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(&atom_0_r1_1, v21, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v8_r3 = v7_r3 + 1;
  atomic_store_explicit(&vars[2], v8_r3, memory_order_release);
  int v10_r5 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v11_r6 = v10_r5 ^ v10_r5;
  atomic_store_explicit(&vars[3+v11_r6], 1, memory_order_release);
  int v13_r9 = atomic_load_explicit(&vars[3], memory_order_acquire);
  int v14_r10 = v13_r9 ^ v13_r9;
  int v17_r11 = atomic_load_explicit(&vars[4+v14_r10], memory_order_acquire);
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  int v22 = (v6_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v18 = atomic_load_explicit(&atom_0_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
