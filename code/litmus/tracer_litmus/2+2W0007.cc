// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/2+2W0007.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 

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
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  int v2_r5 = atomic_load_explicit(&vars[3], memory_order_acquire);
  int v3_r7 = v2_r5 ^ v2_r5;
  atomic_store_explicit(&vars[0+v3_r7], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v4 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v5 = (v4 == 2);
  int v6 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v7 = (v6 == 2);
  int v8_conj = v5 & v7;
  if (v8_conj == 1) MODEL_ASSERT(0);
  return 0;
}
