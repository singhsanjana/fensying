// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO523.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r14_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v5_r4], memory_order_acquire);
  int v9_r7 = v8_r5 ^ v8_r5;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3+v9_r7], 1, memory_order_release);
  int v11_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v12_r11 = v11_r10 ^ v11_r10;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0+v12_r11], 1, memory_order_release);
  int v14_r14 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v21, memory_order_release);
  int v22 = (v14_r14 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r14_1, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r14_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v15 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r14_1, memory_order_acquire);
  int v19_conj = v17 & v18;
  int v20_conj = v16 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
