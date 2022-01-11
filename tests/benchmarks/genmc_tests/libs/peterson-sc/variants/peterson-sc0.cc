#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>
#include "../peterson-sc.c"

int main()
{
	thrd_t t1, t2;

	myinit(&flag1, 0);
	myinit(&flag2, 0);
	myinit(&turn, 0);
	myinit(&x, 0);

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();

	if (pthread_join(t1, NULL))
		abort();
	if (pthread_join(t2, NULL))
		abort();

	return 0;
}
