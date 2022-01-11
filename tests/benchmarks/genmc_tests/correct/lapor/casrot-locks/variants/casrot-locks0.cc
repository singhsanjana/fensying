#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../casrot-locks.c"

int main()
{
	thrd_t t[N];

	for (int i = 1; i < N + 1; i++) {
		idx[i - 1] = i;
		if (pthread_create(&t[i - 1], NULL, thread_n, &idx[i - 1]))
			abort();
	}

	return 0;
}
