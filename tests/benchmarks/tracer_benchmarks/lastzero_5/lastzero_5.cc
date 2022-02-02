/* This benchmark is in the journal JACM 2017
   https://dl.acm.org/citation.cfm?id=3073408
*/

#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#define N 5

atomic_int array[N+1];
int idx[N+1];

static void thread_reader(void *unused)
{
	for (int i = N; atomic_load_explicit(__FILE__, __LINE__, &array[i], memory_order_acquire) != 0; i--)
		;
}

static void thread_writer(void *arg)
{
	int j = *((int *) arg);

	atomic_store_explicit(__FILE__, __LINE__, &array[j],
			      atomic_load_explicit(__FILE__, __LINE__, &array[j - 1], memory_order_acquire) + 1,
			      memory_order_release);
}


int user_main(int argc, char **argv)
{
	thrd_t t[N+1];


	for (int i =0; i <= N; i++)
		atomic_init(&array[i], 0);

	for (int i = 0; i <= N; i++) {
		idx[i] = i;
		if (i == 0) {
			thrd_create(&t[i], (thrd_start_t)&thread_reader, &idx[i]);
		} else {
			thrd_create(&t[i], (thrd_start_t)&thread_writer, &idx[i]);
		}
	}

	return 0;
}
