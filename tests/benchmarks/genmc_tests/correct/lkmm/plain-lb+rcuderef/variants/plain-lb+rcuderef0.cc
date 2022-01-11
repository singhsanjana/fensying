#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int a;

int y;
int *x = &a;

void *P0(void *unused)
{
	int *r0;

	r0 = rcu_dereference(x);
	*r0 = 1;
	return NULL;
}

void *P1(void *unused)
{
	int r0 = READ_ONCE(y);
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
