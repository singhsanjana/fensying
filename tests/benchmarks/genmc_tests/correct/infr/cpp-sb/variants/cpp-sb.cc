// #include <stdlib.h>
#include <threads.h>#include <atomic>

std::atomic<int> x;
std::atomic<int> y;

void *thread_1(void *unused)
{
	x.store(42);
	int r = y.load();
	return nullptr;
}

void *thread_2(void *unused)
{
	y.store(42);
	int r = x.load();
	return nullptr;
}

int main()
{
	thrd_t t1, t2;

	/* Try out some lambdas as well */
	// thrd_create(&t1, (thrd_start_t)& [](void *) -> void* { thread_1(); return nullptr; }, NULL);
	// thrd_create(&t2, (thrd_start_t)& [](void *) -> void * { thread_2(); return nullptr; }, NULL);
	pthread_create(&t1, nullptr, thread_1, nullptr);
	pthread_create(&t2, nullptr, thread_2, nullptr);

	return 0;
}
