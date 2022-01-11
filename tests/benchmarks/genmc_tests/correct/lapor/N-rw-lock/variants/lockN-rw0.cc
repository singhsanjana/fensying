#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>
#include "../lockN-rw.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		idx[i] = i;

	for (int i = 0; i < N; i++) {
		if (pthread_create(&t[i], NULL, threadn, &idx[i]))
			abort();
	}

	return 0;
}
