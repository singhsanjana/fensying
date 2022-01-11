#include <stdlib.h>
#include <threads.h>#include <cassert>

class mutex {
	pthread_mutex_t *_lock;
public:
        mutex(pthread_mutex_t *lock) : _lock(lock) {
		pthread_mutex_lock(_lock);
        }
        ~mutex() {
		pthread_mutex_unlock(_lock);
        }
};

int count = 0;

void t0()
{
	count++;
	return;
}

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	thrd_t threads[1];

	{
		mutex m(&lock);
		auto ret = thrd_create(&threads[0], (thrd_start_t)&[](void*) -> void* { t0(); return nullptr; }, NULL);
		assert(ret == 0);
	}

	{
		mutex m(&lock);
		// pthread_mutex_lock(&lock);
		for (size_t i = 0; i < 1; ++i) {
			auto ret = thrd_join(threads[i]);assert(ret == 0);
		}
		// pthread_mutex_unlock(&lock);
	}

	assert(count == 1);
	return 0;
}
