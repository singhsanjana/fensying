#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int flag;

void *thread_writer(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &flag, 1, memory_order_release);
	;
}

void *thread_reader(void *arg)
{
	int local = 0;
	int count = 0;

	local = atomic_load_explicit(__FILE__, __LINE__, &flag, memory_order_acquire);
	while (local != 1) {
		count++;
		if (count > 40)
			pthread_exit(NULL);
		local = atomic_load_explicit(__FILE__, __LINE__, &flag, memory_order_acquire);
	}
	// printf("got it!\n");
	assert(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) == 42);
	;
}
