#include "hb_3_3.cc"

static void t2(void *arg) {
  b = x.load(__FILE__, __LINE__, memory_order_relaxed);
  atomic_thread_fence(__FILE__, __LINE__, memory_order_release);
  y.store(__FILE__, __LINE__, 1, memory_order_relaxed);
}
