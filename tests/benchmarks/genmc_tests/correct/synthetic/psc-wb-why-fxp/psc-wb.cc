#include "librace.h" 
#include "model-assert.h"
void assume(int);

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	x = 1;

	int a = y;
	assume(a == 0);

	y = 1;

	int b = x;
	assume(b == 1);

	/* y = 0; */
	;
}

void *thread_2(void *unused)
{
	x = 2;

	int a = y;
	assume(a == 0);

	y = 2;

	int b = x;
	assume(b == 2);

	/* y = 0; */
	;
}
