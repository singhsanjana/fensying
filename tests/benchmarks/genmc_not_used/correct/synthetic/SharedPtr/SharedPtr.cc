#include "librace.h" 
#include "model-assert.h"
atomic_int x = ATOMIC_VAR_INIT(3);
atomic_int y = ATOMIC_VAR_INIT(4);

_Atomic(atomic_int *) p;

void *thread_one(void *unused)
{
	int c1 = 0;
	atomic_int * t;

	atomic_store_explicit(__FILE__, __LINE__, &p, &y, memory_order_release);
	for (int i = 0; i < N; i++)
		c1 += atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	t = atomic_load_explicit(__FILE__, __LINE__, &p, memory_order_acquire);
	atomic_store_explicit(t, atomic_load_explicit(__FILE__, __LINE__, t, memory_order_acquire) + 3,
			      memory_order_release);
	/* MODEL_ASSERT(3 <= x && x <= 9); */
	/* MODEL_ASSERT(3 <= y && y <= 9); */
	;
}

void *thread_two(void *unused)
{
	int c2 = 0;
	atomic_int * t;

	atomic_store_explicit(__FILE__, __LINE__, &p, &x, memory_order_release);
	for (int i = 0; i < N; i++)
		c2 += atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	t = atomic_load_explicit(__FILE__, __LINE__, &p, memory_order_acquire);
	atomic_store_explicit(t, atomic_load_explicit(__FILE__, __LINE__, t, memory_order_acquire) + 3,
			      memory_order_release);
	/* MODEL_ASSERT(3 <= x && x <= 9); */
	/* MODEL_ASSERT(3 <= y && y <= 9); */
	;
}
