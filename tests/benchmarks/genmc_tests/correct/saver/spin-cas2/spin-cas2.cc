#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas2: Another CAS spinloop captured by the transformation.
 *            The ordering of the load and the CAS need to match.
 */

atomic_int x;

void *thread_1() /* spawned 3 times */
{
	int r = x;
	while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
		;
	;
}

void *thread_2()
{
	x = 1;
	;
}
