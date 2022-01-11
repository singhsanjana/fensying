#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

atomic_int x;

void *thread_1(void *unused)
{
	while (!x)
		;
	x = 42;
	return NULL;
}

int main()
{
	thrd_t t1;

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();

	return 0;
}
