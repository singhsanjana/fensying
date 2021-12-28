// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/dp3.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_0_r1_0; 
atomic_int atom_1_r1_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v3_r9 = v2_r1 ^ v2_r1;
  atomic_store_explicit(&vars[1+v3_r9], 1, memory_order_release);
  int v10 = (v2_r1 == 0);
  atomic_store_explicit(&atom_0_r1_0, v10, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v6_r9 = v5_r1 ^ v5_r1;
  atomic_store_explicit(&vars[0+v6_r9], 1, memory_order_release);
  int v11 = (v5_r1 == 0);
  atomic_store_explicit(&atom_1_r1_0, v11, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_0, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(&atom_0_r1_0, memory_order_acquire);
  int v8 = atomic_load_explicit(&atom_1_r1_0, memory_order_acquire);
  int v9_disj = v7 | v8;
  if (v9_disj == 0) MODEL_ASSERT(0);
  return 0;
}
