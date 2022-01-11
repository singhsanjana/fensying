#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

/*
 * Result: Sometimes DATARACE
 *
 * Plain loads can be replicated.
 */

int x;
int a = 42;
int b = 42;

void *P0(void *unused)
{
	WRITE_ONCE(x, 1);
	return NULL;
}

void *P1(void *unused)
{
	int r1 = x;
	WRITE_ONCE(a, r1);
	WRITE_ONCE(b, r1);
	return NULL;
}

/* exists (a=0 /\ b=1) */

int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
