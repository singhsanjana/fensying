#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	if (r1 == r2)
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r3 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r3, memory_order_relaxed);
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
