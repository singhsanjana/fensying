#include "librace.h" 
#include "model-assert.h"
atomic<int*> p;

void exit_thread(int *loc)
{
	p.store(__FILE__, __LINE__, loc);
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
	if (p.load(__FILE__, __LINE__) != NULL)
		*(p.load(__FILE__, __LINE__)) = 42;
	;
}
