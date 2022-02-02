/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://svn.sosy-lab.org/software/sv-benchmarks/trunk/c/pthread/fib_bench_false-unreach-call.c */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define NUM 5

// shared variables
atomic_int i, j;

void t1(void *arg)
{
	int _i, _j;
	for (int k=0; k<NUM; k++) {
		_i = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_acquire);
		_j = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &i, _i+_j, memory_order_release);
	}
}

void t2(void *arg)
{
	int _i, _j;
	for (int k=0; k<NUM; k++) {
		_i = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_acquire);
		_j = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &j, _i+_j, memory_order_release);
	}
}

static int fib(int n) {
	int cur = 1, prev = 0;
	while(n--) {
		int next = prev+cur;
		prev = cur;
		cur = next;
	}
	return prev;
}

int user_main(int argc, char **argv)
{
	thrd_t a, b;

	atomic_init(&i, 1);
	atomic_init(&j, 1);

	thrd_create(&a, t1, NULL);
	thrd_create(&b, t2, NULL);

	int correct = fib(2+2*NUM);
	int _i = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_acquire);;
	int _j = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_acquire);

	if (_i > correct || _j > correct) {
		MODEL_ASSERT(0);
	}

	return 0;
}
