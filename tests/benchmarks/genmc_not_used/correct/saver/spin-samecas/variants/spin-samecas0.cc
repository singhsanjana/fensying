#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

/* Validates that we only do DSA for iterations of the same
 * spinloop */

atomic_int x;
atomic_int y;

void spin_until_zero(atomic_int *v)
{
	while (atomic_load_explicit(__FILE__, __LINE__, v, memory_order_relaxed)) {
		/* This will not produce side-effects */
		int r = 17;
		atomic_compare_exchange_strong(__FILE__, __LINE__, &y, &r, 42);
	}
	return;
}

void *thread_1(void *unused)
{
	/* spin once... */
	spin_until_zero(&x);

	/* spin twice! */
	spin_until_zero(&x);
	;
}

void *thread_2(void *unused)
{
	x = 42;
	;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
