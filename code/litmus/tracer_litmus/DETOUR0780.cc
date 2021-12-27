// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0780.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[2]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r6_3; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 4, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(&vars[0], v4_r3, memory_order_release);
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v6_r6 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v13 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v13, memory_order_release);
  int v14 = (v6_r6 == 3);
  atomic_store_explicit(&atom_1_r6_3, v14, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[0], 3, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r6_3, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v7 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v8 = (v7 == 4);
  int v9 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v10 = atomic_load_explicit(&atom_1_r6_3, memory_order_acquire);
  int v11_conj = v9 & v10;
  int v12_conj = v8 & v11_conj;
  if (v12_conj == 1) MODEL_ASSERT(0);
  return 0;
}
