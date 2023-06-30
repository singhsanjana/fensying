#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>

// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR1011.litmus






atomic_int vars[3]; 
atomic_int atom_0_r3_2; 
atomic_int atom_2_r3_0; 

atomic_int __fence_var;

void* t0(void *arg) {
label_1:;
  atomic_store_explicit(  &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(  &vars[1+v3_r4], memory_order_acquire);
  int v7_cmpeq = (v6_r5 == v6_r5);
  if (v7_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(  &vars[2], 1, memory_order_release);
  int v16 = (v2_r3 == 2);
  atomic_store_explicit(  &atom_0_r3_2, v16, memory_order_release);
    return NULL;}

void* t1(void *arg) {
label_2:;
  atomic_store_explicit(  &vars[0], 2, memory_order_release);
    return NULL;}

void* t2(void *arg) {
label_3:;
  atomic_store_explicit(  &vars[2], 2, memory_order_release);
  atomic_fetch_add_explicit(  &__fence_var, 0, memory_order_acq_rel);
  int v9_r3 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v17 = (v9_r3 == 0);
  atomic_store_explicit(  &atom_2_r3_0, v17, memory_order_release);
    return NULL;}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 
  pthread_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);
  pthread_create(&thr2, NULL, t2, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);

  int v10 = atomic_load_explicit(  &vars[2], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(  &atom_0_r3_2, memory_order_acquire);
  int v13 = atomic_load_explicit(  &atom_2_r3_0, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  if (v15_conj == 1) assert(0);
  return 0;
}
