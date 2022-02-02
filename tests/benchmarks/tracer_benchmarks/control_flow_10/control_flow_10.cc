/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Get inspiration from the Control-flow benchmark in Fig 8 in the journal JACM 2017
   https://dl.acm.org/citation.cfm?id=3073408
*/

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define N 10

// shared variables
atomic_int x, y, z;

void p(void *arg)
{
	int loc_x;
	loc_x =  atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

void q(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
}

void r(void *arg)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) == 0)
		atomic_store_explicit(__FILE__, __LINE__, &z, 1, memory_order_release);
}

void s(void *arg)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_acquire) == 1)
		if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) == 0)
			atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
}

int user_main(int argc, char **argv)
{
	int i;
	thrd_t ps[N], qs[N], rs, ss;
	
	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);

	for (i = 0; i < N; i++){
    		thrd_create(&ps[i], p, NULL);
		thrd_create(&qs[i], q, NULL);
  	}

  	thrd_create(&rs, r, NULL);
  	thrd_create(&ss, s, NULL);
  	
	return 0;
}
