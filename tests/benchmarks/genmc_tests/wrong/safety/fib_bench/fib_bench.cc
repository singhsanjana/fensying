#include "librace.h" 
#include "model-assert.h"
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
