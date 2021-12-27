// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/ppc-iwp2.6.litmus

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

atomic_int vars[1]; 
atomic_int atom_2_r5_55; 
atomic_int atom_2_r6_66; 
atomic_int atom_3_r5_66; 
atomic_int atom_3_r6_55; 

atomic_int __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 55, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[0], 66, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r5 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v4_r6 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v16 = (v2_r5 == 55);
  atomic_store_explicit(&atom_2_r5_55, v16, memory_order_release);
  int v17 = (v4_r6 == 66);
  atomic_store_explicit(&atom_2_r6_66, v17, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v6_r5 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v8_r6 = atomic_load_explicit(&vars[0], memory_order_acquire);
  int v18 = (v6_r5 == 66);
  atomic_store_explicit(&atom_3_r5_66, v18, memory_order_release);
  int v19 = (v8_r6 == 55);
  atomic_store_explicit(&atom_3_r6_55, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r5_55, 0); 
  atomic_init(&atom_2_r6_66, 0); 
  atomic_init(&atom_3_r5_66, 0); 
  atomic_init(&atom_3_r6_55, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atomic_load_explicit(&atom_2_r5_55, memory_order_acquire);
  int v10 = atomic_load_explicit(&atom_2_r6_66, memory_order_acquire);
  int v11 = atomic_load_explicit(&atom_3_r5_66, memory_order_acquire);
  int v12 = atomic_load_explicit(&atom_3_r6_55, memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v9 & v14_conj;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
