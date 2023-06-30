/* This benchmark is from RCMC */

/* TRACER is not optimal because it is considering CASs as RMWs */

#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#define N 8

atomic_int x;
int idx[N];

static void thread_n(void *arg)
{
	int newval = *((int *) arg);
	int exp = newval - 1;

	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &x, &exp, newval, memory_order_acq_rel,
						memory_order_acq_rel);
}

int user_main(int argc, char **argv)
{
	thrd_t t[N];

	atomic_init(&x, 0);
	for (int i = 1; i < N + 1; i++) {
		idx[i - 1] = i;
		thrd_create(&t[i - 1], (thrd_start_t)&thread_n, &idx[i - 1]);
	}

	return 0;
}
