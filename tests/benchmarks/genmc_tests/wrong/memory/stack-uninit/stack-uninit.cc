#include "librace.h" 
#include "model-assert.h"
_Atomic(int *) p;
bool done;

void *thread_1(void *unused)
{
	int x;

	p = &x;
	while (!done)
		;
	;
}

void *thread_2(void *unused)
{
	while (p == NULL)
		;

	int r = *p;
	done = 1;
	;
}
