#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

/*
 * Horrible test case, but shows that we have to take the argument
 * into account as well when checking for symmetric threads.
 * (Loading ARG would not work since then SR would fail because
 * the graph would be different, even without checking the arguments.)
 */

#ifndef N
# define N 2
#endif

atomic_int x;
int a[N];

void *thread_n(void *arg)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, (int) arg, memory_order_relaxed);
	;
}

int user_user_user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		a[i] = i;
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
