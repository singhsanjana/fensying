#include <stdio.h>
#include <threads.h>

#include "mcs-lock.h"

/* For data race instrumentation */
#include "librace.h"
#include "model-assert.h"


struct mcs_mutex *mutex;
// static uint32_t shared;
std::atomic<unsigned int> shared;

void threadA(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (g.islocked()) {
		// printf("store: %d\n", 17);
		// store_32(&shared, 17);
		shared.store(__FILE__, __LINE__, 17, std::mo_relaxed);
		mutex->unlock(&g);
		if (mutex->lock(&g)) {
			// printf("load: %u\n", load_32(&shared));
			unsigned int l = shared.load(__FILE__, __LINE__, std::mo_relaxed);
			MODEL_ASSERT(l == 17);
		}
	}
	mutex->unlock(&g);
}

void threadA1(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (g.islocked()) {
		// printf("store: %d\n", 17);
		// store_32(&shared, 17);
		shared.store(__FILE__, __LINE__, 17, std::mo_relaxed);
		unsigned int l = shared.load(__FILE__, __LINE__, std::mo_relaxed);
		MODEL_ASSERT(l == 17);
	}
	mutex->unlock(&g);
}

void threadB1(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (g.islocked()) {
		// printf("store: %d\n", 17);
		// store_32(&shared, 17);
		shared.store(__FILE__, __LINE__, 34, std::mo_relaxed);
		unsigned int l = shared.load(__FILE__, __LINE__, std::mo_relaxed);
		MODEL_ASSERT(l == 34);
	}
	mutex->unlock(&g);
}

void threadB(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (g.islocked()) {
		// printf("load: %u\n", load_32(&shared));
		shared.load(__FILE__, __LINE__, std::mo_relaxed);
	}
	mutex->unlock(&g);
	if (mutex->lock(&g)) {
		// printf("store: %d\n", 17);
		// store_32(&shared, 17);
		shared.store(__FILE__, __LINE__, 17, std::mo_relaxed);
	}
}

int user_main(int argc, char **argv)
{
	thrd_t A, B;

	mutex = new mcs_mutex();
	atomic_init(&shared, 0);

	// thrd_create(&A, &threadA, NULL);
	thrd_create(&A, &threadA1, NULL);
	thrd_create(&B, &threadB1, NULL);
	// thrd_create(&B, &threadB, NULL);
	thrd_join(A);
	thrd_join(B);
	return 0;
}
