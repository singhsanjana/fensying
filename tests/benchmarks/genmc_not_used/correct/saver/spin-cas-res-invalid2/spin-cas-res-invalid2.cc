#include "librace.h" 
#include "model-assert.h"
/*
 * spin-cas-res-invalid2: We cannot apply spin-assume because
 * the loop exists when the CAS fails (as opposed to when it succeeds)
 */

atomic_int x;

void *thread_1()
{
	int r = x;
	while (atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
		;
	;
}

void *thread_2()
{
	x = 1;
	;
}
