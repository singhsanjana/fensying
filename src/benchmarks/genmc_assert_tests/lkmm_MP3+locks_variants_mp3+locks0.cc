#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int r1, r2, r3;

int x, y;
spinlock_t s;

void *P0(void *unused)
{
	spin_lock(&s);
	WRITE_ONCE(x, 1);
	spin_unlock(&s);
	return NULL;
}

void *P1(void *unused)
{
	spin_lock(&s);
	r1 = READ_ONCE(x);
	WRITE_ONCE(y, 1);
	spin_unlock(&s);
	return NULL;
}

void *P2(void *unused)
{
	r2 = READ_ONCE(y);
	smp_rmb();
	r3 = READ_ONCE(x);
	return NULL;
}

int user_main()
{
	thrd_t t0, t1, t2;

	thrd_create(&t0, (thrd_start_t)& P0, NULL);
	thrd_create(&t1, (thrd_start_t)& P1, NULL);
	thrd_create(&t2, (thrd_start_t)& P2, NULL);

	thrd_join(t0);
	thrd_join(t1);
	thrd_join(t2);

	MODEL_ASSERT(!(r1 == 1 && r2 == 1 && r3 == 0));

	return 0;
}
