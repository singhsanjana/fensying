#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../rw.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		if (pthread_create(&t[i], NULL, thread_n, &idx[i]))
			abort();
	}

	return 0;
}
