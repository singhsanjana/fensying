// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/co5.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[1]; 
atomic_int atom_0_r2_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r2 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v7 = (v2_r2 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r2_2, v7, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v4 = (v3 == 2);
  int v5 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r2_2, memory_order_acquire);
  int v6_conj = v4 & v5;
  if (v6_conj == 1) MODEL_ASSERT(0);
  return 0;
}