#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define LOOP_LIMIT 2

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> z;

static void fn1(void* arg)
{
	x.store(__FILE__, __LINE__, 1, memory_order_relaxed);
    atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	for (int i = 0; i < LOOP_LIMIT; i++)
		y.store(__FILE__, __LINE__, 1, memory_order_relaxed);

    
}

static void fn2(void* arg)
{
	int cnt = 0;
	int ly = y.load(__FILE__, __LINE__, memory_order_relaxed);
	int lx = x.load(__FILE__, __LINE__, memory_order_relaxed);
    while ((ly == 0 || lx == 0) && cnt < LOOP_LIMIT) cnt++;
		
	ly = y.load(__FILE__, __LINE__, memory_order_relaxed);
	lx = x.load(__FILE__, __LINE__, memory_order_relaxed);
	if (ly == 1 && lx == 1) {
        y.store(__FILE__, __LINE__, 2, memory_order_relaxed);
        z.store(__FILE__, __LINE__, 1, memory_order_relaxed);
    }

    
}

static void fn3(void* arg)
{
    int a = z.load(__FILE__, __LINE__, memory_order_relaxed);
    int b = x.load(__FILE__, __LINE__, memory_order_relaxed);

	MODEL_ASSERT(! (a == 1 && b == 0));	

    
}

int user_main(int argc, char **argv)
{
	thrd_t t1,t2,t3;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);

	thrd_create(&t1, (thrd_start_t)&fn2, NULL);
	thrd_create(&t2, (thrd_start_t)&fn1, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

	return 0;
}
