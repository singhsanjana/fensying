#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas7: Invalid PHI in the CAS header (load result is manipulated).
 *            Transformation fails in this case.
 */

atomic_int x;

void *thread_1()
{
	int r = x + 1;
	while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
		;
	;
}

void *thread_2()
{
	x = 1;
	;
}
