#include <threads.h>#include <atomic>
#include <cMODEL_ASSERT>

// using namespace std;

std::atomic<int> x(0);
std::atomic<int> y(0);

void t0()
{
	y.store(20, std::memory_order_release);
	x.store(10, std::memory_order_release);
}

void t1()
{
	if (x.load(std::memory_order_acquire) == 10) {
		MODEL_ASSERT(y.load(std::memory_order_acquire) == 20);
	}
}

int user_main()
{
	thrd_t threads[2];

	thrd_create(&threads[0], (thrd_start_t)&[](void*) -> void* {t0(); return nullptr;}, NULL);
	thrd_create(&threads[1], (thrd_start_t)&[](void*) -> void* {t1(); return nullptr;}, NULL);
	for (auto i = 0; i < 2; ++i) {
		thrd_join(threads[i]);
	}

	return 0;
}
