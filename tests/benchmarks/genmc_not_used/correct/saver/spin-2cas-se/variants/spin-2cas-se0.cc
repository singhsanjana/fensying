#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

/*
 * In this test case one path leadins to the loop's only latch
 * potentially has side effects, but we can still validate
 * that this is a spinloop dynamically
*/

atomic_int x;

void *thread_1(void *unused)
{
	int r;

	r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	for (;;) {
		if (r == 42) {
			if (atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
				break;
		} else {
			atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 17);
		}
	}
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
