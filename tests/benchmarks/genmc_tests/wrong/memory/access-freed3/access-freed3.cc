#include "librace.h" 
#include "model-assert.h"
_Atomic(int *) p;

void *thread1(void *unused)
{
	p = malloc(sizeof(int));
	*p = 42;
	free(p);
	;
}

void *thread2(void *unused)
{
	p = malloc(sizeof(int));
	;
}

void *thread3(void *unused)
{
	int *r = p;
	if (r)
		*r = 18;
	;
}
