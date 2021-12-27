// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S+PPO329.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
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
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(&vars[2+v3_r3], memory_order_acquire);
  int v8_r6 = atomic_load_explicit(&vars[2], memory_order_acquire);
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  int v10_r8 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v11_r9 = v10_r8 ^ v10_r8;
  int v14_r10 = atomic_load_explicit(&vars[3+v11_r9], memory_order_acquire);
  int v15_r12 = v14_r10 ^ v14_r10;
  int v16_r12 = v15_r12 + 1;
  atomic_store_explicit(&vars[0], v16_r12, memory_order_release);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v17 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
