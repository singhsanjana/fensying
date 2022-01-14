#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas3: CAS spinloop captured by the transformation.
 *            Models the acquisition and release of a lock.
 */

atomic_int x;

void *thread_n()
{
	int r = 0;
	while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 1))
		r = 0;
	x = 0;
	;
}
