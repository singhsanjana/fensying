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
	WRITE_ONCE(x, 1);
	WRITE_ONCE(y, 1);
	rcu_read_unlock();
	return NULL;
}

void *P1(void *unused)
{
	r_x = READ_ONCE(x);
	synchronize_rcu();
	r_y = READ_ONCE(y);
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

	MODEL_ASSERT(!(r_x == 1 && r_y == 0));

	return 0;
}
