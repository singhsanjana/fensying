// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/Z6.1+rfi-data+lwsync+data.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r3_2; 
atomic_int atom_2_r1_1; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v4_r4 = v3_r4 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], v4_r4, memory_order_release);
  int v18 = (v2_r3 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r3_2, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v8_r3 = v7_r3 + 1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], v8_r3, memory_order_release);
  int v19 = (v6_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r3_2, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  int v17_conj = v10 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
