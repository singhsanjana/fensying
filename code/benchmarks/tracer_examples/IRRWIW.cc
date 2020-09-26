// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/IRRWIW.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_3_r1_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(__LINE__, 2, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v2_r1 = vars0.load(__LINE__, memory_order_acquire);
  int v4_r3 = vars1.load(__LINE__, memory_order_acquire);
  int v15 = (v2_r1 == 2);
  atom_1_r1_2.store(__LINE__, v15, memory_order_release);
  int v16 = (v4_r3 == 0);
  atom_1_r3_0.store(__LINE__, v16, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars1.store(__LINE__, 1, memory_order_release);

}

static void t3(void *arg){
label_4:;
  int v6_r1 = vars1.load(__LINE__, memory_order_acquire);
  vars0.store(__LINE__, 1, memory_order_release);
  int v17 = (v6_r1 == 1);
  atom_3_r1_1.store(__LINE__, v17, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v7 = vars0.load(__LINE__, memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atom_1_r1_2.load(__LINE__, memory_order_acquire);
  int v10 = atom_1_r3_0.load(__LINE__, memory_order_acquire);
  int v11 = atom_3_r1_1.load(__LINE__, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  int v14_conj = v8 & v13_conj;
  if (v14_conj == 1) MODEL_ASSERT(0);
  return 0;
}