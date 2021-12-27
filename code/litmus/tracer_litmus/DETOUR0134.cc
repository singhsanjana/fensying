// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0134.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r5_1; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r4_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_store_explicit(&vars[1], 1, memory_order_release);
  int v2_r5 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  int v14 = (v2_r5 == 1);
  atomic_store_explicit(&atom_0_r5_1, v14, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;
  int v8_r4 = atomic_load_explicit(&vars[0+v5_r3], memory_order_acquire);
  int v15 = (v4_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v15, memory_order_release);
  int v16 = (v8_r4 == 0);
  atomic_store_explicit(&atom_1_r4_0, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r5_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(&atom_0_r5_1, memory_order_acquire);
  int v10 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v11 = atomic_load_explicit(&atom_1_r4_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
