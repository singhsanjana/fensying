// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/LB+PPO0549.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_0_r1_1; 
atomic_int atom_1_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(&vars[1], v4_r3, memory_order_release);
  int v22 = (v2_r1 == 1);
  atomic_store_explicit(&atom_0_r1_1, v22, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(&vars[2+v7_r3], memory_order_acquire);
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  int v12_r7 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v13_r8 = v12_r7 ^ v12_r7;
  int v16_r9 = atomic_load_explicit(&vars[3+v13_r8], memory_order_acquire);
  int v18_r11 = atomic_load_explicit(&vars[3], memory_order_acquire);
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  int v23 = (v6_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

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

  int v19 = atomic_load_explicit(&atom_0_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v21_conj = v19 & v20;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
