// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/WW+RW+RR+WR+WR+lwsync+sync+lwsync+sync+sync.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[5]; 
atomic_int atom_1_r1_1; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r3_0; 
atomic_int atom_3_r3_0; 
atomic_int atom_4_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v20, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v4_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v6_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v21 = (v4_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_1, v21, memory_order_release);
  int v22 = (v6_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r3_0, v22, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v8_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[4], memory_order_acquire);
  int v23 = (v8_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_3_r3_0, v23, memory_order_release);

}

void t4(void *arg){
label_5:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[4], 1, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v10_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v24 = (v10_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_4_r3_0, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 
  thrd_t thr4; 

  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&atom_3_r3_0, 0); 
  atomic_init(&atom_4_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);
  thrd_create(&thr4, t4, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);
  thrd_join(thr4);

  int v11 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r3_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_3_r3_0, memory_order_acquire);
  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_4_r3_0, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  int v19_conj = v11 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
