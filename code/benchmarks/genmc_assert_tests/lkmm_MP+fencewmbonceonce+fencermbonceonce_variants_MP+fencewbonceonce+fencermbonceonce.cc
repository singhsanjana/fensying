#include <stdlib.h>
#include <threads.h>#include <lkmm.h>
#include "librace.h" 
#include "model-assert.h"

int x;
int y;

int r0, r1;

void *P0(void *unused)
{
	WRITE_ONCE(x, 1);
	smp_wmb();
	WRITE_ONCE(y, 1);
	return NULL;
}

void *P1(void *unused)
{
	r0 = READ_ONCE(y);
	smp_rmb();
	r1 = READ_ONCE(x);
	return NULL;
}

int user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& P0, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& P1, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();

	MODEL_ASSERT(!(r0 == 1 && r1 == 0));

	return 0;
}
