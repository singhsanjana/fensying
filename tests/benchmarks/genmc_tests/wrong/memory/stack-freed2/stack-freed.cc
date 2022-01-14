#include "librace.h" 
#include "model-assert.h"
_Atomic(int *) p;

void exit_thread(int *loc)
{
	p = loc;
	pthread_exit(NULL);
}

void *thread1(void *unused)
{
	int x = 0;
	exit_thread(&x);
	;
}

void *thread2(void *unused)
{
	if (p != NULL)
		*p = 42;
	;
}
