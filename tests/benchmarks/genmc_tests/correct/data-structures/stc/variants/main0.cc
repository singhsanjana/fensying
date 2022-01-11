#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>#include <stdbool.h>
#include "librace.h" 
#include "model-assert.h"

#include "../main.c"

int main()
{
	thrd_t t1, t2, t3;

	for (int i = 0u; i <= 3; i++)
		idx[i] = i;
	if (pthread_create(&t1, NULL, thread_1, &idx[1]))
		abort();
	if (pthread_create(&t2, NULL, thread_2, &idx[2]))
		abort();
	if (pthread_create(&t3, NULL, thread_3, &idx[3]))
		abort();

	return 0;
}
