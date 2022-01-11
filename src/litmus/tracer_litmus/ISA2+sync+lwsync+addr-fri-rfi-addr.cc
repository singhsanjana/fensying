// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ISA2+sync+lwsync+addr-fri-rfi-addr.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[4]; 
atomic_int atom_1_r1_1; 
atomic_int atom_2_r1_1; 
atomic_int atom_2_r4_0; 
atomic_int atom_2_r7_1; 
atomic_int atom_2_r9_0; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(&vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(&vars[2], 1, memory_order_release);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(&atom_1_r1_1, v24, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v4_r1 = atomic_load_explicit(&vars[2], memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;
  int v8_r4 = atomic_load_explicit(&vars[3+v5_r3], memory_order_acquire);
  atomic_store_explicit(&vars[3], 1, memory_order_release);
  int v10_r7 = atomic_load_explicit(&vars[3], memory_order_acquire);
  int v11_r8 = v10_r7 ^ v10_r7;
  int v14_r9 = atomic_load_explicit(&vars[0+v11_r8], memory_order_acquire);
  int v25 = (v4_r1 == 1);
  atomic_store_explicit(&atom_2_r1_1, v25, memory_order_release);
  int v26 = (v8_r4 == 0);
  atomic_store_explicit(&atom_2_r4_0, v26, memory_order_release);
  int v27 = (v10_r7 == 1);
  atomic_store_explicit(&atom_2_r7_1, v27, memory_order_release);
  int v28 = (v14_r9 == 0);
  atomic_store_explicit(&atom_2_r9_0, v28, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&atom_2_r7_1, 0); 
  atomic_init(&atom_2_r9_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v15 = atomic_load_explicit(&atom_1_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(&atom_2_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(&atom_2_r4_0, memory_order_acquire);
  int v18 = atomic_load_explicit(&atom_2_r7_1, memory_order_acquire);
  int v19 = atomic_load_explicit(&atom_2_r9_0, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23_conj = v15 & v22_conj;
  if (v23_conj == 1) MODEL_ASSERT(0);
  return 0;
}
