#include "librace.h" 
#include "model-assert.h"
void assume(int);

atomic_int x;

void *thread1(void *unused)
{
	x = 1;
	int a = atomic_fetch_add(&x, 1);
	assume(a == 42);
	;
}

void *thread2(void *unused)
{
	int b = atomic_fetch_add(&x, 42);
	assume(b == 0);

	/* If all assume()s hold, this will have nowhere to read-from */
	int r = x;
	;
}
