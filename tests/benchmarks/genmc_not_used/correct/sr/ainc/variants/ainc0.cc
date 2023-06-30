#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

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
	;
}

int user_user_user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);

	return 0;
}
