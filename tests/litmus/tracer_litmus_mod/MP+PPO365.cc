// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO365.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 
atomic_int atom_1_r11_0; 

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
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[2+v3_r3], memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &vars[2], 1, memory_order_release);
  int v8_r7 = atomic_load_explicit(__FILE__, __LINE__, &vars[2], memory_order_acquire);
  int v9_cmpeq = (v8_r7 == v8_r7);
  if (v9_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[3], 1, memory_order_release);
  int v11_r10 = atomic_load_explicit(__FILE__, __LINE__, &vars[3], memory_order_acquire);
  int v12_cmpeq = (v11_r10 == v11_r10);
  if (v12_cmpeq)  goto lbl_LC01; else goto lbl_LC01;
lbl_LC01:;
  atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v14_r11 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v18 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r1_1, v18, memory_order_release);
  int v19 = (v14_r11 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r11_0, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r11_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v15 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r11_0, memory_order_acquire);
  int v17_conj = v15 & v16;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
