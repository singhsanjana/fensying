#include "librace.h" 
#include "model-assert.h"
_Atomic(int *) p;

void *thread1(void *unused)
{
	int x = 0;
	p = &x;
	;
}

void *thread2(void *unused)
{
	if (p != NULL)
		*p = 42;
	;
}
