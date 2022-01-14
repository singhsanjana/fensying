// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/2+2W0048.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v3_r4], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11_conj = v8 & v10;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}