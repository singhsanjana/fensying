#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

#include "../commits.c"

int main()
{
	thrd_t t[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; i++) {
		idx[i] = i + 1;
		if (pthread_create(&t[i], NULL, thread_n, &idx[i]))
			abort();
	}

	return 0;
}
