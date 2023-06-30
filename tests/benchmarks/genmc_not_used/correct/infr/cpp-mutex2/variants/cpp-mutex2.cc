#include "librace.h" 
#include "model-assert.h"
#include <mutex>
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

void t0(void *l)
{
	mutex lock((std::mutex *) l);
	count++;;
	return;
}

static std::mutex lock = PTHREAD_MUTEX_INITIALIZER;

int user_main()
{
	thrd_t threads[2];

	auto ret = thrd_create(&threads[0], (thrd_start_t)&[](void *) -> void* { t0((void *) , NULL);
	MODEL_ASSERT(ret == 0);
	ret = thrd_create(&threads[1], (thrd_start_t)&[](void *) -> void* { t0((void *) , NULL);
	MODEL_ASSERT(ret == 0);

	ret = thrd_join(threads[0]);
	MODEL_ASSERT(ret == 0);
	ret = thrd_join(threads[1]);
	MODEL_ASSERT(ret == 0);

	MODEL_ASSERT(count == 2);
	return 0;
}
