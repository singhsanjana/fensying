// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO151.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r14_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v3_r3], memory_order_acquire);
  int v7_r6 = v6_r4 ^ v6_r4;
  int v8_r6 = v7_r6 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], v8_r6, memory_order_release);
  int v10_r8 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v11_r9 = v10_r8 ^ v10_r8;
  int v14_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[4+v11_r9], memory_order_acquire);
  int v16_r12 = atomic_load_explicit(__FILE__, __LINE__, &vars[4], memory_order_acquire);
  int v17_r13 = v16_r12 ^ v16_r12;
  int v20_r14 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v17_r13], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v24, memory_order_release);
  int v25 = (v20_r14 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r14_0, v25, memory_order_release);

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
  atomic_init(&atom_1_r14_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v21 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r14_0, memory_order_acquire);
  int v23_conj = v21 & v22;
  if (v23_conj == 1) MODEL_ASSERT(0);
  return 0;
}
