#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include <genmc.h>
#include "librace.h" 
#include "model-assert.h"

#include "../main.c"

int main()
{
	thrd_t t[N];

	ticketlock_init(&lock);
	for (int i = 0u; i < N; i++) {
		if (pthread_create(&t[i], NULL, thread_n, NULL))
			abort();
	}

	return 0;
}
