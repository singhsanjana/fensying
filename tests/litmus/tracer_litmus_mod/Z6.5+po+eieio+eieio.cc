// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/Z6.5+po+eieio+eieio.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_2_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v10 = (v2_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r3_0, v10, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v4 = (v3 == 2);
  int v5 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r3_0, memory_order_acquire);
  int v8_conj = v6 & v7;
  int v9_conj = v4 & v8_conj;
  if (v9_conj == 1) MODEL_ASSERT(0);
  return 0;
}
