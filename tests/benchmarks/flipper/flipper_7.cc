#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define LOOP 7

atomic<int> x;
atomic<int> y;

static void fn1(void* arg)
{
	x.store(__FILE__, __LINE__, 1,memory_order_relaxed);
	
}

static void fn2(void* arg)
{
	if (x.load(__FILE__, __LINE__, memory_order_relaxed)) {
		for (int i = 0; i < LOOP; i++){
		        int m = 0;
	        	int n = 1;
			y.compare_exchange_strong(__FILE__, __LINE__, m, n, memory_order_relaxed);
    		}
	}
	
}

static void fn3(void* arg)
{
	if (y.load(__FILE__, __LINE__, memory_order_relaxed)) {
		int a = x.load(__FILE__, __LINE__, memory_order_relaxed);
		MODEL_ASSERT(!(a == 0));
	}
	
}

static void fn4(void* arg)
{
	for (int i = 0; i < LOOP; i++){
        	int m = 0;
	        int n = 1;
        	y.compare_exchange_strong(__FILE__, __LINE__, n, m, memory_order_relaxed);
 	}
	
}

int user_main(int argc, char **argv)
{
	thrd_t t1,t2,t3,t4;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	thrd_create(&t1, (thrd_start_t)&fn2, NULL);
	thrd_create(&t2, (thrd_start_t)&fn1, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);
	thrd_create(&t4, (thrd_start_t)&fn4, NULL);
    
	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

	return 0;
}
