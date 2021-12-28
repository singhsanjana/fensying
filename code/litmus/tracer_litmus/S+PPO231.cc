// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S+PPO231.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(&vars[2+v5_r4], memory_order_acquire);
  int v10_r7 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v11_r8 = v10_r7 ^ v10_r7;
  atomic_store_explicit(&vars[0+v11_r8], 1, memory_order_release);
  int v16 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v12 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v15_conj = v13 & v14;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
