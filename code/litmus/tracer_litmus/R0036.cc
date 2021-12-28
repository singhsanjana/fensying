// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/R0036.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_1_r7_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  atomic_store_explicit(&vars[2+v3_r4], 1, memory_order_release);
  int v5_r7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v10 = (v5_r7 == 0);
  atomic_store_explicit(&atom_1_r7_0, v10, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v6 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7 = (v6 == 2);
  int v8 = atomic_load_explicit(&atom_1_r7_0, memory_order_acquire);
  int v9_conj = v7 & v8;
  if (v9_conj == 1) MODEL_ASSERT(0);
  return 0;
}
