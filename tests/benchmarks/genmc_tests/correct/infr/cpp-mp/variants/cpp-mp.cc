#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <atomic>
#include <cassert>

// using namespace std;

std::atomic<int> x(0);
std::atomic<int> y(0);

void t0()
{
	y.store(__FILE__, __LINE__, 20, std::memory_order_release);
	x.store(__FILE__, __LINE__, 10, std::memory_order_release);
}

void t1()
{
	if (x.load(__FILE__, __LINE__, std::memory_order_acquire) == 10) {
		assert(y.load(__FILE__, __LINE__, std::memory_order_acquire) == 20);
	}
}

int user_user_main()
{
	thrd_t threads[2];

	thrd_create(&threads[0], (thrd_start_t)&[](void*) -> void* {t0(); return nullptr;}, NULL);
	thrd_create(&threads[1], (thrd_start_t)&[](void*) -> void* {t1(); return nullptr;}, NULL);
	for (auto i = 0; i < 2; ++i) {
		pthread_join(threads[i], nullptr);
	}

	return 0;
}
