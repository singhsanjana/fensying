#include "librace.h" 
#include "model-assert.h"
#define THREAD_NUM 2

atomic_int n = ATOMIC_VAR_INIT(N);
atomic_int x;

int idx[THREAD_NUM];

void *thread_n(void *arg)
{
	int i, j = *((int *) arg);

	i = 0;
	while (i < atomic_load_explicit(__FILE__, __LINE__, &n, memory_order_acquire)) {
		/* Stamp */
		atomic_store_explicit(__FILE__, __LINE__, &x, j, memory_order_release);
		/* Do stuff, then read stamp */
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) == j)
			++i; /* Commit was successful */
	}
	;
}
