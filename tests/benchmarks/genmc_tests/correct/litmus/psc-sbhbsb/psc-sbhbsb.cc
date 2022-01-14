#include "librace.h" 
#include "model-assert.h"
/*
 * This test case exposes a bug in the implementation where
 * a PSC edge from Wx1 to Ry0 was added in the following program,
 * despite the fact that this should not be the case:
 *
 *    Wx1 (SC) || Rx1 (ACQ)
 *    Wy1 (SC) || Ry0 (SC)
 *
 * An sb_(<>loc);hb;sb_(<>loc) edge was erroneously added because
 * it was checked whether Wx1 is hb-before Rx1, instead of
 * checking whether Wy1 is hb-before Rx1.
 *
 * (Print PSC to see the erroneously added edge.)
 */

void assume(int);

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	x = 1;
	y = 1;
	;
}

void *thread_2(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	assume(a == 1);

	int b = y;
	assume(b == 0);
	;
}
