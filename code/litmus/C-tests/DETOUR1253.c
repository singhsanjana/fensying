/* Copyright (C) 2018 Magnus Lång and Tuan Phong Ngo
 * This benchmark is part of SWSC */

#include <assert.h>
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_int vars[3]; 
atomic_int atom_1_r3_2; 

void *t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_seq_cst);

  atomic_store_explicit(&vars[1], 1, memory_order_seq_cst);
  return NULL;
}

void *t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_seq_cst);
  int v2_r3 = atomic_load_explicit(&vars[1], memory_order_seq_cst);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(&vars[2+v3_r4], memory_order_seq_cst);
  int v7_r7 = v6_r5 ^ v6_r5;
  int v8_r7 = v7_r7 + 1;
  atomic_store_explicit(&vars[0], v8_r7, memory_order_seq_cst);
  int v16 = (v2_r3 == 2);
  atomic_store_explicit(&atom_1_r3_2, v16, memory_order_seq_cst);
  return NULL;
}

int main(int argc, char *argv[]){
  pthread_t thr0; 
  pthread_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r3_2, 0);

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);

  int v9 = atomic_load_explicit(&vars[1], memory_order_seq_cst);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(&vars[0], memory_order_seq_cst);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(&atom_1_r3_2, memory_order_seq_cst);
  int v14_conj = v12 & v13;
  int v15_conj = v10 & v14_conj;
  if (v15_conj == 1) assert(0);
  return 0;
}
