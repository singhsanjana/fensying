#include <stdlib.h>
#include <threads.h>#include <stdbool.h>
#include <stdatomic.h>

#include "../filesystem.c"

int main()
{
	thrd_t t[N];


	for (int i = 0; i < N; i++)
		idx[i] = i;
	for (int i = 0; i < N; i++) {
		if (pthread_create(&t[i], NULL, thread_n, &idx[i]))
			abort();
	}

	return 0;
}
