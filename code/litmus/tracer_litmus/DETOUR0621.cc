// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0621.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r7_2; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 3, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v13 = (v2_r3 == 0);
  atomic_store_explicit(&atom_0_r3_0, v13, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v4_r3 = atomic_load_explicit(&vars[2], memory_order_acquire);
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  int v6_r7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v14 = (v6_r7 == 2);
  atomic_store_explicit(&atom_1_r7_2, v14, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r7_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v7 = atomic_load_explicit(&atom_0_r3_0, memory_order_acquire);
  int v8 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v9 = (v8 == 3);
  int v10 = atomic_load_explicit(&atom_1_r7_2, memory_order_acquire);
  int v11_conj = v9 & v10;
  int v12_conj = v7 & v11_conj;
  if (v12_conj == 1) MODEL_ASSERT(0);
  return 0;
}
