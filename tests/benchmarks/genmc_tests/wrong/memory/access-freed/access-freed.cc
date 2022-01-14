#include "librace.h" 
#include "model-assert.h"
#define mo_acq memory_order_acquire

_Atomic(int *) p;

void *thread1(void *unused)
{
	p = malloc(sizeof(int));
	free(p);
	;
}

void *thread2(void *unused)
{
	int *r = atomic_load_explicit(__FILE__, __LINE__, &p, mo_acq);
	if (r) {
		*r = 18;
	}
	;
}
