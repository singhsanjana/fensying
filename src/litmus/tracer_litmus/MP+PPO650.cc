// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO650.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r7_1; 

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
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  int v4_r4 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r5 = v4_r4 ^ v4_r4;
  int v6_r5 = v5_r5 + 1;
  atomic_store_explicit(&vars[0], v6_r5, memory_order_release);
  int v8_r7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v18 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v18, memory_order_release);
  int v19 = (v8_r7 == 1);
  atomic_store_explicit(&atom_1_r7_1, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r7_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v14 = atomic_load_explicit(&atom_1_r7_1, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  int v17_conj = v10 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
