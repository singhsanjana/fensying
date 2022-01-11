#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"
#include <genmc.h>

#include "../main.c"

int main()
{
	thrd_t t[N];

	for (int i = 0u; i < N; i++)
		pthread_create(&t[i], NULL, thread_n, (void *) i);

	return 0;
}
