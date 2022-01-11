#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include <genmc.h>
#include "librace.h" 
#include "model-assert.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "../cr-ul+li-ul.c"

int main()
{
	thrd_t t1, t2;

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();

	return 0;
}
