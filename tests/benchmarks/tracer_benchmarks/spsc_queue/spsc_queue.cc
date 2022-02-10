/* This benchmark is from CDSChecker */

/* Modified to test with RCMC and TRACER */



#include <threads.h>

#include "queue.h"


#define L 1

spsc_queue<int> *q;

void thread1(void *arg)
{
	for (int i=0; i<L; i++)
		q->enqueue(i);
}

void thread2(void *arg)
{
	q->dequeue();
}

int user_main(int argc, char **argv)
{
	thrd_t A, B;

	q = new spsc_queue<int>();
	
	thrd_create(&A, thread1, NULL);
	thrd_create(&B, thread2, NULL);
	
	return 0;
}
