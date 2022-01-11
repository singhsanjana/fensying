#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int x;
atomic_t y;

void *P0(void *unused)
{
	WRITE_ONCE(x, 1);
	int r0 = atomic_xchg_release(&y, 5);
	return NULL;
}

void *P1(void *unused)
{
	atomic_inc(&y);
	smp_mb();
	int r1 = READ_ONCE(x);
	return NULL;
}

int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
