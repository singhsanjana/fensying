/* C C-MPrelseq+o-r+rmwinc+a-o */

#include <stdlib.h>
#include <threads.h>#include <lkmm.h>
#include <genmc.h>

atomic_t x1;
int x0 = 0;

void *P0(void *unused)
{
	WRITE_ONCE(x0, 2);
	atomic_set_release(&x1, 2);
	return NULL;
}

void *P1(void *unused)
{
	atomic_inc(&x1);
	return NULL;
}

void *P2(void *unused)
{
	int r2 = atomic_read_acquire(&x1);
	int r3 = READ_ONCE(x0);
	return NULL;
}

int main()
{
	thrd_t t0;
	thrd_t t1;
	thrd_t t2;

	if (pthread_create(&t0, NULL, P0, NULL))
		abort();
	if (pthread_create(&t1, NULL, P1, NULL))
		abort();
	if (pthread_create(&t2, NULL, P2, NULL))
		abort();

	return 0;
}
