#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define N 2

static std::mutex lock;

static atomic_int x = {0};
static int readX = 0;

static void *runLock(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	lock.lock();
	;
}

static void *runUnlock(void *arg)
{
	readX = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	if (readX == 0)
		lock.unlock();
	;
}
