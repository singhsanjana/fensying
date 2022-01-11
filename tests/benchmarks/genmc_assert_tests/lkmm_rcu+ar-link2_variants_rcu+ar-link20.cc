#include <stdlib.h>
#include <lkmm.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

int x;
int y;
int a;
int s;

int r0;
int r2;
int r6;

void *P0(void *unused)
{
	rcu_read_lock();
	WRITE_ONCE(x, 2);
	r0 = READ_ONCE(y);
	rcu_read_unlock();
	return NULL;
}

void *P1(void *unused)
{
	WRITE_ONCE(x, 1);
	smp_mb();
	WRITE_ONCE(s, 1);
	return NULL;
}

void *P2(void *unused)
{
	int r = READ_ONCE(s);
	if (r) {
		r2 = r;
		WRITE_ONCE(a, 2);
	}
	return NULL;
}

void *P3(void *unused)
{
	WRITE_ONCE(a, 1);
	synchronize_rcu();
	WRITE_ONCE(y, 1);
	return NULL;
}

int user_main()
{
	thrd_t t0, t1, t2, t3;

	thrd_create(&t0, (thrd_start_t)& P0, NULL);
	thrd_create(&t1, (thrd_start_t)& P1, NULL);
	thrd_create(&t2, (thrd_start_t)& P2, NULL);
	thrd_create(&t3, (thrd_start_t)& P3, NULL);

	thrd_join(t0);
	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

	MODEL_ASSERT(!(r0 == 1 && r2 == 1 && a == 1 && x == 1));

	return 0;
}
