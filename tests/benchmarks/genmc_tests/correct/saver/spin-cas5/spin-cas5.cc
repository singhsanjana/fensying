#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas5: PHI nodes for a counter in the loop header.
 *            The transformation should fail.
 */

atomic_int x;

void *thread_1()
{
	int a = 0;
	int r = x;

	while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
		a++;
	;
}

void *thread_2()
{
	x = 1;
	;
}
