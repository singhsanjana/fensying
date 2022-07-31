#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

#define ROUND 25

using namespace std;

atomic<int> x;
atomic<int> y;
/*
atomic<int> a = {0};
atomic<int> b = {ROUND};
atomic<int> c = {0};
atomic<int> d = {ROUND};
*/

int a = 0;
int b = ROUND;
int c = 0;
int d = ROUND;

static void increment(void *arg) 
{
	for (int i = 0; i < ROUND; i++)
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
}

static void decrement(void *arg) 
{
	for (int i = 0; i < ROUND; i++)
		atomic_fetch_sub_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
}

static void read(void *arg) 
{
	a = x.load(__FILE__, __LINE__, memory_order_relaxed);
	b = y.load(__FILE__, __LINE__, memory_order_relaxed);
}

static void read2(void *arg) 
{
	c = y.load(__FILE__, __LINE__, memory_order_relaxed);
	d = x.load(__FILE__, __LINE__, memory_order_relaxed);
}

int user_main(int argc, char **argv)
{
	thrd_t t1,t2,t3,t4;

	atomic_init(&x, 0);
	atomic_init(&y, ROUND);

	thrd_create(&t1, (thrd_start_t)&increment, NULL);
	thrd_create(&t2, (thrd_start_t)&decrement, NULL);
	thrd_create(&t3, (thrd_start_t)&read, NULL);
	thrd_create(&t4, (thrd_start_t)&read2, NULL);
    
	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

	MODEL_ASSERT(!(a > 0 && b == ROUND && c < ROUND && d == 0));

	return 0;
}
