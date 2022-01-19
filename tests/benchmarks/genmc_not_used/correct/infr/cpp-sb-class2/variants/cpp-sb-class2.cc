#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <atomic>

class SB {

public:
	SB() {
		x = new std::atomic<int>(0);
		y = new std::atomic<int>(0);
	}

	~SB() {
		delete x;
		delete y;
	}

	std::atomic<int> *x;
	std::atomic<int> *y;
};

void *thread_1(void *arg)
{
	auto *sb = (SB *) arg;

	sb->x->store(42);
	sb->y->load();
	return nullptr;
}

void *thread_2(void *arg)
{
	auto *sb = (SB *) arg;

	sb->y->store(42);
	sb->x->load();
	return nullptr;
}

int user_user_main()
{
	thrd_t t1, t2;

	SB vars;

	thrd_create(&t1, (thrd_start_t)& thread_1, NULL);
	thrd_create(&t2, (thrd_start_t)& thread_2, NULL);

	pthread_join(t1, nullptr);
	pthread_join(t2, nullptr);

	return 0;
}
