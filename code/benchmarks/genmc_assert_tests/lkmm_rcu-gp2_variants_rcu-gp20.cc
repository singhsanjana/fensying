#include <stdlib.h>
#include <threads.h>#include <lkmm.h>
#include "librace.h" 
#include "model-assert.h"

int x;
int y;

int r_x;
int r_y;

void *P0(void *unused)
{
	rcu_read_lock();
	r_x = READ_ONCE(x);
	r_y = READ_ONCE(y);
	rcu_read_unlock();
	return NULL;
}

void *P1(void *unused)
{
	WRITE_ONCE(x, 1);
	synchronize_rcu();
	WRITE_ONCE(y, 1);
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

	MODEL_ASSERT(!(r_x == 0 && r_y == 1));

	return 0;
}
