// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-cookbook6.2.1.2.noloop.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r3_1; 
atomic_int atom_1_r5_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 55, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r12 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v3_r12 = v2_r12 & 0;
  int v6_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v3_r12], memory_order_acquire);
  int v10 = (v2_r12 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r3_1, v10, memory_order_release);
  int v11 = (v6_r5 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r5_0, v11, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r3_1, memory_order_acquire);
  int v8 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r5_0, memory_order_acquire);
  int v9_conj = v7 & v8;
  if (v9_conj == 1) MODEL_ASSERT(0);
  return 0;
}
