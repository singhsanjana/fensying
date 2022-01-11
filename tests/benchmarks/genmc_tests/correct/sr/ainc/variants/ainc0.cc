#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

/*
 * Symmetry reduction cannot be applied for this one, due to memory
 * accesses generated between the creation of the threads.
 * These memory accesses are created to access the t[N] array.
 */

#ifndef N
# define N 2
#endif

atomic_int x;

void *thread_n(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	return NULL;
}

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		if (pthread_create(&t[i], NULL, thread_n, NULL))
			abort();

	return 0;
}
