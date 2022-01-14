#include "librace.h" 
#include "model-assert.h"
/*
 * We can spin-assume for this one, as exit from the while loop
 * happens when one of the CASes succeeds
 */

atomic_int x;

void *thread_1(void *unused)
{
	int success = 0;
	while (!success) {
		int r = 0;
		int p = 1;
		if (x == 42)
			success = atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42);
		else
			success = atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &p, 42);
	}
	;
}

void *thread_2(void *unused)
{
	for (int i = 0u; i < 42; i++)
		x = i;
	;
}
