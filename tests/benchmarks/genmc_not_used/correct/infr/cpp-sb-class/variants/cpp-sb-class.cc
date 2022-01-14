#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <atomic>

std::atomic<int> x;
std::atomic<int> y;

class SB1 {

public:
	SB1() {
		x.store(__FILE__, __LINE__, 42);
		val = y.load(__FILE__, __LINE__);
	}

private:
	int val;
};

class SB2 {

public:
	SB2() {
		y.store(__FILE__, __LINE__, 42);
		val = x.load(__FILE__, __LINE__);
	}

private:
	int val;
};

void *thread_1(void *unused)
{
	SB1 t1;
	return nullptr;
}

void *thread_2(void *unused)
{
	SB2 t2;
	return nullptr;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	thrd_create(&t1, (thrd_start_t)& thread_1, NULL);
	thrd_create(&t2, (thrd_start_t)& thread_2, NULL);

	return 0;
}
