#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

/* Result: Never */

int x;
spinlock_t s;

void *P0(void *unused)
{
	spin_lock(&s);
	WRITE_ONCE(x, 1);
	return NULL;
}

void *P1(void *unused)
{
	int r1;

	spin_lock(&s);
	r1 = x;
	spin_unlock(&s);
	return NULL;
}

/* exists (1:r1=1) */

int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
