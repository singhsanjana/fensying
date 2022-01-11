#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int a;

int y;
int *x = &a;

void *P0(void *unused)
{
	int *r0 = rcu_dereference(x);
	int r1 = *r0;
	return NULL;
}

void *P1(void *unused)
{
	WRITE_ONCE(y, 1);
	rcu_assign_pointer(x, &y);
	return NULL;
}

int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
