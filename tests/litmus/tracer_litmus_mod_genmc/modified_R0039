#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>

// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/R0039.litmus






atomic_int vars[4]; 
atomic_int atom_1_r7_0; 

atomic_int __fence_var;

void* t0(void *arg) {
label_1:;
  atomic_store_explicit(  &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(  &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(  &vars[1], 1, memory_order_release);
    return NULL;}

void* t1(void *arg) {
label_2:;
  atomic_store_explicit(  &vars[1], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(  &vars[2], memory_order_acquire);
  int v3_r5 = v2_r3 ^ v2_r3;
  int v4_r5 = v3_r5 + 1;
  atomic_store_explicit(  &vars[3], v4_r5, memory_order_release);
  int v6_r7 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v11 = (v6_r7 == 0);
  atomic_store_explicit(  &atom_1_r7_0, v11, memory_order_release);
    return NULL;}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&__fence_var, 0); 

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);

  int v7 = atomic_load_explicit(  &vars[1], memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atomic_load_explicit(  &atom_1_r7_0, memory_order_acquire);
  int v10_conj = v8 & v9;
  if (v10_conj == 1) assert(0);
  return 0;
}
