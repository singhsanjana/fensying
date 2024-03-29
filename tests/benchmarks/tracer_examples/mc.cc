/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r2_0; 
atomic<int> atom_1_r2_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 3, memory_order_release);
  int v2_r2 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v26 = (v2_r2 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r2_0, v26, memory_order_release);
  int v27 = (v2_r2 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_1_r2_1, v27, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v3 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r2_0, memory_order_acquire);
  int v4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v5 = (v4 == 3);
  int v6 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v7 = (v6 == 1);
  int v8_conj = v5 & v7;
  int v9_conj = v3 & v8_conj;
  int v10 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r2_0, memory_order_acquire);
  int v11 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v12 = (v11 == 3);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15_conj = v12 & v14;
  int v16_conj = v10 & v15_conj;
  int v17 = atomic_load_explicit(__FILE__, __LINE__, &atom_1_r2_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v19 = (v18 == 3);
  int v20 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v21 = (v20 == 2);
  int v22_conj = v19 & v21;
  int v23_conj = v17 & v22_conj;
  int v24_disj = v16_conj | v23_conj;
  int v25_disj = v9_conj | v24_disj;
  if (v25_disj == 1) MODEL_ASSERT(0);
  return 0;
}
