#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>

// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0552.litmus






atomic_int vars[2]; 
atomic_int atom_0_r3_0; 
atomic_int atom_1_r3_1; 
atomic_int atom_1_r6_1; 

atomic_int __fence_var;

void* t0(void *arg) {
label_1:;
  atomic_store_explicit(  &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(  &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(  &vars[1], memory_order_acquire);
  int v17 = (v2_r3 == 0);
  atomic_store_explicit(  &atom_0_r3_0, v17, memory_order_release);
    return NULL;}

void* t1(void *arg) {
label_2:;
  atomic_store_explicit(  &vars[1], 1, memory_order_release);
  int v4_r3 = atomic_load_explicit(  &vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v6_r4 = v5_r4 + 1;
  atomic_store_explicit(  &vars[0], v6_r4, memory_order_release);
  int v8_r6 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v18 = (v4_r3 == 1);
  atomic_store_explicit(  &atom_1_r3_1, v18, memory_order_release);
  int v19 = (v8_r6 == 1);
  atomic_store_explicit(  &atom_1_r6_1, v19, memory_order_release);
    return NULL;}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r6_1, 0); 
  atomic_init(&__fence_var, 0); 

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);

  int v9 = atomic_load_explicit(  &atom_0_r3_0, memory_order_acquire);
  int v10 = atomic_load_explicit(  &vars[0], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(  &atom_1_r3_1, memory_order_acquire);
  int v13 = atomic_load_explicit(  &atom_1_r6_1, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) assert(0);
  return 0;
}
