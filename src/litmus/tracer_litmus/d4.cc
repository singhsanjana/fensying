// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/d4.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r1 = v2_r1 + 1;
  atomic_store_explicit(&vars[0], v3_r1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v6_r1 = v5_r1 + 2;
  atomic_store_explicit(&vars[1], v6_r1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v8 = (v7 == 0);
  int v9 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v10 = (v9 == 0);
  int v11_conj = v8 & v10;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}
