#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, r1, memory_order_relaxed);
	return NULL;
}

void *thread_3(void *unused)
{
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	int r3 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	return NULL;
}

int main()
{
	thrd_t t1, t2, t3;

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();
	if (pthread_create(&t3, NULL, thread_3, NULL))
		abort();

	return 0;
}
