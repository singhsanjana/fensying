// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/dd4.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_2_r2_1; 
atomic_int atom_3_r2_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r2 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r10 = v2_r2 ^ v2_r2;
  atomic_store_explicit(&vars[0+v3_r10], 1, memory_order_release);
  int v10 = (v2_r2 == 1);
  atomic_store_explicit(&atom_2_r2_1, v10, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v5_r2 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v6_r10 = v5_r2 ^ v5_r2;
  atomic_store_explicit(&vars[1+v6_r10], 1, memory_order_release);
  int v11 = (v5_r2 == 1);
  atomic_store_explicit(&atom_3_r2_1, v11, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r2_1, 0); 
  atomic_init(&atom_3_r2_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v7 = atomic_load_explicit(&atom_2_r2_1, memory_order_acquire);
  int v8 = atomic_load_explicit(&atom_3_r2_1, memory_order_acquire);
  int v9_conj = v7 & v8;
  if (v9_conj == 1) MODEL_ASSERT(0);
  return 0;
}
