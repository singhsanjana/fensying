#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../big1.c"

int main()
{
	thrd_t t1, t2, t3, t4;

	if (pthread_create(&t4, NULL, thread_four, NULL))
		abort();
	if (pthread_create(&t1, NULL, thread_one, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_two, NULL))
		abort();
	if (pthread_create(&t3, NULL, thread_three, NULL))
		abort();

	return 0;
}
