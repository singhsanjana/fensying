#include <stdlib.h>
#include <threads.h>
#include "../corr0-sc.c"

int main()
{
	thrd_t t1, t2, t3;

	myinit(&x, 0);

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_3, NULL))
		abort();
	if (pthread_create(&t3, NULL, thread_2, NULL))
		abort();

	if (pthread_join(t1, NULL))
		abort();
	if (pthread_join(t3, NULL))
		abort();
	if (pthread_join(t2, NULL))
		abort();

	return 0;
}
