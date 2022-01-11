#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

atomic_int x;

void *thread_n(void *unused)
{
	x = 1;
	int r = x;
	x = 2;
	return NULL;
}

int main()
{
	thrd_t t1, t2;

	if (pthread_create(&t1, NULL, thread_n, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_n, NULL))
		abort();

	return 0;
}
