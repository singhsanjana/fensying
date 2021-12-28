// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/CO-R.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[1]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r1_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_store_explicit(&vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[0], 3, memory_order_release);
  int v2_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v12 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v12, memory_order_release);
  int v13 = (v2_r1 == 2);
  atomic_store_explicit(&atom_1_r1_2, v13, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v3 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v4 = (v3 == 3);
  int v5 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v6 = atomic_load_explicit(&atom_1_r1_2, memory_order_acquire);
  int v7_disj = v5 | v6;
  int v8_conj = v4 & v7_disj;
  int v9 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v10 = (v9 == 1);
  int v11_disj = v8_conj | v10;
  if (v11_disj == 1) MODEL_ASSERT(0);
  return 0;
}
