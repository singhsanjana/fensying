// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0666.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[3]; 
atomic_int atom_0_r3_2; 
atomic_int atom_2_r3_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v14 = (v2_r3 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r3_2, v14, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 2, memory_order_release);
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v7_r3 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v15 = (v7_r3 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r3_0, v15, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v8 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r3_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r3_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
