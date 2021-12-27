// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO909.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r13_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__LINE__, &vars[2+v3_r3], memory_order_acquire);
  int v8_r6 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v9_r7 = v8_r6 ^ v8_r6;
  int v12_r8 = atomic_load_explicit(__LINE__, &vars[3+v9_r7], memory_order_acquire);
  int v13_r10 = v12_r8 ^ v12_r8;
  int v14_r10 = v13_r10 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v14_r10, memory_order_release);
  int v16_r12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v18_r13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v25 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v25, memory_order_release);
  int v26 = (v18_r13 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r13_1, v26, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r13_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v19 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v20 = (v19 == 2);
  int v21 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_1_r13_1, memory_order_acquire);
  int v23_conj = v21 & v22;
  int v24_conj = v20 & v23_conj;
  if (v24_conj == 1) MODEL_ASSERT(0);
  return 0;
}
