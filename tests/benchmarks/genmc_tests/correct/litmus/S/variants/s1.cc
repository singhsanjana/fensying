#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../s.c"

int main()
{
	thrd_t t1, t2;

	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();
	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();

	return 0;
}
