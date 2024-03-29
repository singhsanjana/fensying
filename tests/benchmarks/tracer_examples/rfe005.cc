/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_2_r1_2; 
atomic<int> atom_2_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1+v3_r3], 1, memory_order_release);
  int v18 = (v2_r1 == 1);
  atomic_store_explicit(__FILE__, __LINE__, &atom_0_r1_1, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[1], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v5_r1 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v6_r3 = v5_r1 ^ v5_r1;
  int v9_r4 = atomic_load_explicit(__FILE__, __LINE__, &vars[0+v6_r3], memory_order_acquire);
  int v19 = (v5_r1 == 2);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r1_2, v19, memory_order_release);
  int v20 = (v9_r4 == 0);
  atomic_store_explicit(__FILE__, __LINE__, &atom_2_r4_0, v20, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__FILE__, __LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v10 = atomic_load_explicit(__FILE__, __LINE__, &vars[1], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(__FILE__, __LINE__, &atom_0_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r1_2, memory_order_acquire);
  int v14 = atomic_load_explicit(__FILE__, __LINE__, &atom_2_r4_0, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  int v17_conj = v11 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
