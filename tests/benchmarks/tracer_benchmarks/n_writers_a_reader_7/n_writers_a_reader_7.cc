/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* CDSChecker example 
 * https://dl.acm.org/citation.cfm?id=2914585.2806886, page 10, section 5.4
 * There are N+1 weak traces
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"


#define N 7

// shared variables
atomic_int x;

void writer(void *arg) {
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
}



void reader(void *arg) {
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}




int user_main(int argc, char **argv)
{
	thrd_t ws[N], r;
	atomic_init(&x, 0);

	for (int i=0; i<N; i++) {
		thrd_create(&ws[i], writer, NULL);
	}

	thrd_create(&r, reader, NULL);

	return 0;
}
