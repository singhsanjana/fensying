#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

#include "../stack-1.c"

int main(void)
{
	thrd_t id1, id2;

	if (pthread_mutex_init(&m, NULL))
		abort();

	if (pthread_create(&id1, NULL, t1, NULL))
		abort();
	if (pthread_create(&id2, NULL, t2, NULL))
		abort();

	if (pthread_join(id1, NULL))
		abort();
	if (pthread_join(id2, NULL))
		abort();

	return 0;
}
