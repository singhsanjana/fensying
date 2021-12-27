// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S+PPO019.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
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
  int v9_r7 = v8_r6 ^ v8_r6;
  int v10_r7 = v9_r7 + 1;
  atomic_store_explicit(&vars[3], v10_r7, memory_order_release);
  int v12_r9 = atomic_load_explicit(&vars[3], memory_order_acquire);
  int v13_r10 = v12_r9 ^ v12_r9;
  int v16_r11 = atomic_load_explicit(&vars[4+v13_r10], memory_order_acquire);
  int v17_r13 = v16_r11 ^ v16_r11;
  atomic_store_explicit(&vars[0+v17_r13], 1, memory_order_release);
  int v22 = (v2_r1 == 1);
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
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v18 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v19 = (v18 == 2);
  int v20 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v21_conj = v19 & v20;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
