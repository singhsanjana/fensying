#include "librace.h" 
#include "model-assert.h"
// also for acq
#define mo_rlx memory_order_relaxed

_Atomic(int *) p;

void *thread1(void *unused)
{
	p = malloc(sizeof(int));
	//*p = 17;
	/* free(p); */
	;
}

void *thread2(void *unused)
{
	int *r = atomic_load_explicit(__FILE__, __LINE__, &p, mo_rlx);
	if (r) {
		*r = 18;
	}
	;
}
