#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <stdatomic.h>
atomic_int i ;
atomic_int j ;

#define NUM 2

void *t1(void* arg)
{
	for (int k = 0; k < NUM; k++) {
		int prevI = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_relaxed);
		int prevJ = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_relaxed);
		atomic_store_explicit(__FILE__, __LINE__, &i, prevI + prevJ, memory_order_relaxed);
	}
	
	return NULL;
}

void *t2(void* arg)
{
	for (int k = 0; k < NUM; k++) {
		int prevI = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_relaxed);
		int prevJ = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_relaxed);
		atomic_store_explicit(__FILE__, __LINE__, &j, prevI + prevJ, memory_order_relaxed);
	}
	
	return NULL;
}

void *t3(void *arg)
{
	int temp1 = atomic_load_explicit(__FILE__, __LINE__, &i, memory_order_relaxed) ;
	int temp2 = atomic_load_explicit(__FILE__, __LINE__, &j, memory_order_relaxed);
	if (temp1 >= 8 ||  temp2 >= 8)
		MODEL_ASSERT(0);
	
	return NULL;
}
int user_main(int argc, char **argv)
{
	thrd_t id1, id2, id3;
	atomic_init(&i, 1);
	atomic_init(&j, 1);

	thrd_create(&id2, (thrd_start_t)& t2, NULL);
	thrd_create(&id1, (thrd_start_t)& t1, NULL);
	thrd_create(&id3, (thrd_start_t)& t3, NULL);

	return 0;
}