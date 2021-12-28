// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0399.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r3_3; 
atomic_int atom_2_r5_2; 
atomic_int atom_2_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(&vars[1+v3_r4], memory_order_acquire);
  int v7_r7 = v6_r5 ^ v6_r5;
  atomic_store_explicit(&vars[2+v7_r7], 1, memory_order_release);
  int v19 = (v2_r3 == 3);
  atomic_store_explicit(&atom_0_r3_3, v19, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[0], 3, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v9_r1 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v10_r3 = v9_r1 ^ v9_r1;
  int v11_r3 = v10_r3 + 1;
  atomic_store_explicit(&vars[0], v11_r3, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v13_r5 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v20 = (v13_r5 == 2);
  atomic_store_explicit(&atom_2_r5_2, v20, memory_order_release);
  int v21 = (v9_r1 == 1);
  atomic_store_explicit(&atom_2_r1_1, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_3, 0); 
  atomic_init(&atom_2_r5_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v14 = atomic_load_explicit(&atom_0_r3_3, memory_order_acquire);
  int v15 = atomic_load_explicit(&atom_2_r5_2, memory_order_acquire);
  int v16 = atomic_load_explicit(&atom_2_r1_1, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
