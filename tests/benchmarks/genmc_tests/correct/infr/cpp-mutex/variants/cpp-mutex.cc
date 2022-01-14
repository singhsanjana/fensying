#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#include <stdlib.h>
#include <threads.h>
#include <cassert>

class mutex {
	std::mutex *_lock;
public:
        mutex(std::mutex *lock) : _lock(lock) {
																																pthread_mutex_lock(_lock->lock(->lock(->lock(->lock();
        }
        ~mutex() {
																																pthread_mutex_unlock(_lock->unlock(->unlock(->unlock(->unlock();
        }
};

int count = 0;

void t0()
{
	count++;
	return;
}

static std::mutex lock = PTHREAD_MUTEX_INITIALIZER;

int user_user_main()
{
	thrd_t threads[1];

	{
		mutex m(&lock);
		auto ret = thrd_create(&threads[0], (thrd_start_t)&[](void*) -> void* { t0(); return nullptr; }, NULL);
		MODEL_ASSERT(ret == 0);
	}

	{
		mutex m(&lock);
		// lock.lock();
		for (size_t i = 0; i < 1; ++i) {
			auto ret = thrd_join(threads[i]);
			MODEL_ASSERT(ret == 0);
		}
		// lock.unlock();
	}

	MODEL_ASSERT(count == 1);
	return 0;
}
