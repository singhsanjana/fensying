#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../lockinc.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		if (pthread_create(&t[i], NULL, thread_n, NULL))
			abort();

	return 0;
}
