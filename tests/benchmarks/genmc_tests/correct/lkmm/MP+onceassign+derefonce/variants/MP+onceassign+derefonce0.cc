#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int x;
int z;
int *y = &z;

void *P0(void *unused)
{
	WRITE_ONCE(x, 1);
	rcu_assign_pointer(y, &x);
	return NULL;
}

void *P1(void *unused)
{
	rcu_read_lock();
	int *r0 = rcu_dereference(y);
	int r1 = READ_ONCE(*r0);
	rcu_read_unlock();
	return NULL;
}


int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
