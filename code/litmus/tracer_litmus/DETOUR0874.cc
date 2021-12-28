// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0874.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r3_2; 
atomic_int atom_1_r6_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  int v5_r6 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v15 = (v2_r3 == 2);
  atomic_store_explicit(&atom_1_r3_2, v15, memory_order_release);
  int v16 = (v5_r6 == 1);
  atomic_store_explicit(&atom_1_r6_1, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r6_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v6 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7 = (v6 == 2);
  int v8 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(&atom_1_r3_2, memory_order_acquire);
  int v11 = atomic_load_explicit(&atom_1_r6_1, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  int v14_conj = v7 & v13_conj;
  if (v14_conj == 1) MODEL_ASSERT(0);
  return 0;
}
