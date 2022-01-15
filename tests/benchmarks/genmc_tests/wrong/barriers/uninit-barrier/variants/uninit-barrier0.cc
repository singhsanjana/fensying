#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

pthread_barrier_t b;

int user_main(int argc, char **argv)
{
	pthread_barrier_wait(&b);
	return 0;
}
