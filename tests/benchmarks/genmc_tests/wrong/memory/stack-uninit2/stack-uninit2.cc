#include "librace.h" 
#include "model-assert.h"
/*
 * Shows why checking whether reading from uninitialized memory
 * only when adding a load is not sufficient. (For this to be
 * visible, the driver needs to select another rf apart from INIT
 * as the first rf; e.g., by modifying the mocalc.)
 * Thus, such checks need to be performed when restring the
 * graph as well.
 */

_Atomic(atomic_int *) p;
bool done;

void *thread_1(void *unused)
{
	atomic_int x;

	atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &p, &x, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	while (p == NULL)
		;

	atomic_int r = *p;
	done = 1;
	;
}
