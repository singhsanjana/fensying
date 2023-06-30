/* 
 * Adapted from: https://svn.sosy-lab.org/software/sv-benchmarks/trunk/c/pthread/fib_bench_true-unreach-call.c 
 * Written by Magnus Lang
*/

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define IJ(I, J) ((uint64_t)(I) << 32 | (J))
#define I(ij) ((uint32_t)((ij) >> 32))
#define J(ij) ((uint32_t)((ij) & (((uint64_t)2<<32)-1)))

atomic<uint64_t> ij;

#define N 1

void t1(void* arg) {
    for (int k = 0; k < N; k++){
      // i+=j;
      uint64_t tmpj = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
      uint64_t tmpi = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
      atomic_store_explicit(__FILE__, __LINE__, &ij, IJ(I(tmpi)+J(tmpj), J(tmpj)), memory_order_release);
    }
}

void t2(void* arg) {
    for (int k = 0; k < N; k++) {
      // j+=i;
      uint64_t tmpi = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
      uint64_t tmpj = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
      atomic_store_explicit(__FILE__, __LINE__, &ij, IJ(I(tmpi), I(tmpi)+J(tmpj)), memory_order_release);
    }
  }

static int fib(int n) {
    int cur = 1, prev = 0;
    while(n--) {
      int next = prev+cur;
      prev = cur;
      cur = next;
    }
    return prev;
}

int user_main(int argc, char *argv[]) {
    thrd_t id1, id2;
  
    atomic_init(&ij, IJ(1, 1));
  
    thrd_create(&id1, t1, NULL);
    thrd_create(&id2, t2, NULL);

    int correct = fib(2+2*N);
    uint64_t tmpi = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
    uint64_t tmpj = atomic_load_explicit(__FILE__, __LINE__, &ij, memory_order_acquire);
    if (I(tmpi) > correct || J(tmpj) > correct) {
      MODEL_ASSERT(0);
    }

    return 0;
}
