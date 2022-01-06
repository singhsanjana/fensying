#include <stdlib.h>
#include <threads.h>#include <lkmm.h>
#include "librace.h" 
#include "model-assert.h"

/*
 * Buggy test case. Demonstrates an illegal rcu behavior, but with a
 * po; ar; pb; prop; po cycle between the RCU-RSCS and the GP. It also
 * shows why the ar* component in the beginning of rcu-link is necessary
 * (otherwise, the BUG_ON() fires).
 * Finally, to get the correct number of executions for this test case,
 * we need to include rcu-fence in strong-fence (in other words, this
 * test case demonstrates how redefinition in herd affect previously
 * affected relations).
 */

#define BUG_ON(x) MODEL_ASSERT(!(x))

/* RCU variables */
int x;
int y;

int r_x;
int r_y;

/* helper variables */
int s;
int w[2] = {0, 1};

int r_s;
int r_w;

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
	if (READ_ONCE(x) == 1)
		WRITE_ONCE(s, 1);
	return NULL;
}

void *P2(void *unused)
{
	r_s = READ_ONCE(s); /* ar from x up to this point */
	r_w = READ_ONCE(w[r_s]); /* prop to P3 */
	return NULL;
}

void *P3(void *unused)
{
	WRITE_ONCE(w[1], 2);
	synchronize_rcu();
	r_y = READ_ONCE(y);
	return NULL;
}

int user_main()
{
	thrd_t t0, t1, t2, t3;

	if (thrd_create(&t0, (thrd_start_t)& P0, NULL))
		abort();
	if (thrd_create(&t1, (thrd_start_t)& P1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& P2, NULL))
		abort();
	if (thrd_create(&t3, (thrd_start_t)& P3, NULL))
		abort();

	if (thrd_join(t0))
		abort();
	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();
	if (thrd_join(t3))
		abort();

	BUG_ON(r_y == 0 && r_s == 1 && r_w == 1);

	return 0;
}
