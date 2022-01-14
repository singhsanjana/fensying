#include "librace.h" 
#include "model-assert.h"
void assume(int);

atomic_int x;
atomic_int y;
atomic_int z;

void *thread1(void *unused)
{
	x = 1;
	y = 1;
	assume(z == 0);
	;
}

void *thread2(void *unused)
{
	z = 1;
	assume(y == 2);
	;
}

void *thread3(void *unused)
{
	y = 2;
	assume(x == 0);
	;
}
