#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>

// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/SB0027.litmus






atomic_int vars[3]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r7_1; 

atomic_int __fence_var;

void* t0(void *arg) {
label_1:;
  atomic_store_explicit(  &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(  &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(  &vars[1], memory_order_acquire);
  int v14 = (v2_r3 == 0);
  atomic_store_explicit(  &atom_0_r3_0, v14, memory_order_release);
    return NULL;}

void* t1(void *arg) {
label_2:;
  atomic_store_explicit(  &vars[1], 1, memory_order_release);
  int v4_r3 = atomic_load_explicit(  &vars[2], memory_order_acquire);
  int v5_cmpeq = (v4_r3 == v4_r3);
  if (v5_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(  &vars[0], 1, memory_order_release);
  int v7_r7 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v15 = (v7_r7 == 1);
  atomic_store_explicit(  &atom_1_r7_1, v15, memory_order_release);
    return NULL;}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r7_1, 0); 
  atomic_init(&__fence_var, 0); 

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);

  int v8 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(  &atom_0_r3_0, memory_order_acquire);
  int v11 = atomic_load_explicit(  &atom_1_r7_1, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) assert(0);
  return 0;
}
