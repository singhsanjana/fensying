#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>
#include "../lockN-ww.c"

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		idx[i] = i;

	for (int i = 0; i < N; i++) {
		if (pthread_create(&t[i], NULL, threadn, &idx[i]))
			abort();
	}
	for (int i = 0; i < N; i++) {
		if (pthread_join(t[i], NULL))
			abort();
	}

	assert (x == y);

	return 0;
}
