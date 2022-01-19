#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas8: Different memory orders between the read before the CAS and the CAS.
 * Transformation should fail.
 */

atomic_int x;

void *thread_1()
{
	int r = x;
	while (!atomic_compare_exchange_weak_explicit(__FILE__, __LINE__&x, &r, 42, memory_order_acq_rel,
						      memory_order_acq_rel))
		;
	;
}

void *thread_2()
{
	x = 1;
	;
}
