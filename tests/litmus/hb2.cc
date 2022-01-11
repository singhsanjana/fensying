/**
 * misc - eg1-sw
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;
int a,b,c;

static void t1(void *arg) {
  a = z.load(__FILE__, __LINE__, memory_order_relaxed);
  x.store(__FILE__, __LINE__, 1, memory_order_relaxed);
}

static void t2(void *arg) {
  b = x.load(__FILE__, __LINE__, memory_order_relaxed);
  atomic_thread_fence(__FILE__, __LINE__, memory_order_acquire);
  y.store(__FILE__, __LINE__, 1, memory_order_relaxed);
}