#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>#include <stdbool.h>
#include <stdatomic.h>

#include "../indexer.c"

int idx[N];

int main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		pthread_create(&t[i], NULL, thread_n, &idx[i]);
	}

	return 0;
}
