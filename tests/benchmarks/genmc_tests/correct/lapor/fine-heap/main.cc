#include "librace.h" 
#include "model-assert.h"
#include "fine-heap.cc"

/* Driver code */
#ifndef N
# define N 2
#endif

#ifndef MAX_THREADS
# define MAX_THREADS 32
#endif

static int num_threads;
int param[MAX_THREADS + 1];
thrd_t threads[MAX_THREADS + 1];

int __thread tid;

void set_thread_num(int i)
{
	tid = i;
}

int get_thread_num()
{
	return tid;
}

DEFINE_HEAP(myheap);

void *thread_n(void *tid)
{
	int t = (*(int *) tid);
	set_thread_num(t);

	if (t % 2 == 0)
		add(&myheap, t * 2, t);
	else
		add(&myheap, t / 2, t);
	;
}
