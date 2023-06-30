#include "librace.h" 
#include "model-assert.h"
/*
 * We cannot apply spin-assume here since the CAS uses a different
 * compare argument at each loop iteration
 */

atomic_int x;

void *thread_1(void *unused)
{
	int success = 0;
	int r = 0;
	while (!(success = atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42)))
		;
	;
}

void *thread_2(void *unused)
{
	for (int i = 0u; i < 42; i++)
		x = i;
	;
}
