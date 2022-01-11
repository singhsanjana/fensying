#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int x;
int y;

void *P0(void *unused)
{
	WRITE_ONCE(x, 1);
	synchronize_rcu();
	WRITE_ONCE(y, 1);
	return NULL;
}

void *P1(void *unused)
{
	int r1 = READ_ONCE(y);
	WRITE_ONCE(x, 2);
	return NULL;
}


int main()
{
	thrd_t t0, t1;

	pthread_create(&t0, NULL, P0, NULL);
	pthread_create(&t1, NULL, P1, NULL);

	return 0;
}
