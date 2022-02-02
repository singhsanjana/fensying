/* This benchmark is from RCMC */

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#define N 5
atomic_int x;
int idx[N];

static void thread_n(void *arg)
{
	int r = 0, val = *((int *) arg);

	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &x, &r, 1, memory_order_acq_rel,
						memory_order_acq_rel);
	atomic_store_explicit(__FILE__, __LINE__, &x, val + 3, memory_order_release);
}

int user_main(int argc, char **argv)
{
	thrd_t t[N];

	atomic_init(&x, 0);
	for (int i = 0; i < N; i++) {
		idx[i] = i;
		thrd_create(&t[i], (thrd_start_t)&thread_n, &idx[i]);
	}

	return 0;
}
