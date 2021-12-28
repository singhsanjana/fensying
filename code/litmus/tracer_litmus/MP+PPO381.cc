// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO381.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r9_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  atomic_store_explicit(&vars[2+v5_r4], 1, memory_order_release);
  atomic_store_explicit(&vars[2], 2, memory_order_release);
  int v7_r8 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v8_cmpeq = (v7_r8 == v7_r8);
  if (v8_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v10_r9 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v17 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v17, memory_order_release);
  int v18 = (v10_r9 == 0);
  atomic_store_explicit(&atom_1_r9_0, v18, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r9_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v11 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v14 = atomic_load_explicit(&atom_1_r9_0, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
