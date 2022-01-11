#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../cas.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i + 1;
		if (pthread_create(&t[i], NULL, thread_n, &idx[i]))
			abort();
	}

	return 0;
}
