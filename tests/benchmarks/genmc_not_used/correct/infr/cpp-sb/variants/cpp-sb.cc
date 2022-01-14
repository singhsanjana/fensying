#include "librace.h" 
#include "model-assert.h"
// #include <stdlib.h>
#include <threads.h>
#include <atomic>

std::atomic<int> x;
std::atomic<int> y;

void *thread_1(void *unused)
{
	x.store(__FILE__, __LINE__, 42);
	int r = y.load(__FILE__, __LINE__);
	return nullptr;
}

void *thread_2(void *unused)
{
	y.store(__FILE__, __LINE__, 42);
	int r = x.load(__FILE__, __LINE__);
	return nullptr;
}

int user_user_main()
{
	thrd_t t1, t2;

	/* Try out some lambdas as well */
	// thrd_create(&t1, (thrd_start_t)& [](void *) -> void* { thread_1(); return nullptr; }, NULL);
	// thrd_create(&t2, (thrd_start_t)& [](void *) -> void * { thread_2(); return nullptr; }, NULL);
	thrd_create(&t1, (thrd_start_t)& thread_1, NULL);
	thrd_create(&t2, (thrd_start_t)& thread_2, NULL);

	return 0;
}
