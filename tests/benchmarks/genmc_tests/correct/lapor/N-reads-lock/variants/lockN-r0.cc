#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>
#include "../lockN-r.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		if (pthread_create(&t[i], NULL, threadn, NULL))
			abort();
	}

	return 0;
}
