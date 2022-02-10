/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* My example: Get insprration from example in Figure 5, 
 * Data-centric dynamic partial order reduction
 *  https://dl.acm.org/citation.cfm?id=3158119
 * There are 3N^2+3N+1 weak traces
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define N 5

// shared variables
atomic_int x;

void thread1(void *arg) {
	for (int i=0; i<N; i++)
		atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
}

void thread2(void *arg) {
	for (int i=0; i<N; i++)
		atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
}

void thread3(void *arg) {
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3;
	atomic_init(&x, 0);

	thrd_create(&t1, thread1, NULL);
	thrd_create(&t3, thread3, NULL);
	thrd_create(&t2, thread2, NULL);

	return 0;
}
