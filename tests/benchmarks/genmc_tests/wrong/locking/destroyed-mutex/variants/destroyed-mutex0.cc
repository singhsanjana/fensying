#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include <genmc.h>

atomic_int x;
atomic_int y;

pthread_mutex_t l;

void *thread_1(void *unused)
{
	pthread_mutex_destroy(&l);

	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	return NULL;
}

void *thread_2(void *unused)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire))
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed))
			pthread_mutex_lock(&l);
	return NULL;
}

int main()
{
	thrd_t t1, t2;

	pthread_mutex_init(&l, NULL);

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();

	return 0;
}
