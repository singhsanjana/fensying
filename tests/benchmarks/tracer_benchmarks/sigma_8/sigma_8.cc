/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://raw.githubusercontent.com/sosy-lab/sv-benchmarks/master/c/pthread/sigma_false-unreach-call.c */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define SIGMA 8

// shared variables
atomic_int array[SIGMA]; 
atomic_int array_index;

void thread(void *arg) {
	int _array_index;
	_array_index = atomic_load_explicit(__FILE__, __LINE__, &array_index, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, &array[_array_index], 1, memory_order_release);
}

int user_main(int argc, char **argv)
{
	int _array_index;
	thrd_t tids[SIGMA];
	atomic_init(&array_index, -1);
	
	for (int i = 0; i < SIGMA; i++){
		atomic_init(&array[i], 0);
	}

	for (int i = 0; i < SIGMA; i++){
		_array_index = atomic_load_explicit(__FILE__, __LINE__, &array_index, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &array_index, _array_index+1, memory_order_release);
		thrd_create(&tids[i], thread, NULL);
	}

	return 0;
}
