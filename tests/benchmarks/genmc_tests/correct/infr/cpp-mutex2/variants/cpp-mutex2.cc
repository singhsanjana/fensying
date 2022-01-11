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

void t0(void *l)
{
	mutex lock((pthread_mutex_t *) l);
	count++;;
	return;
}

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	thrd_t threads[2];

	auto ret = thrd_create(&threads[0], (thrd_start_t)&[](void *) -> void* { t0((void *) , NULL);
	assert(ret == 0);
	ret = thrd_create(&threads[1], (thrd_start_t)&[](void *) -> void* { t0((void *) , NULL);
	assert(ret == 0);

	ret = thrd_join(threads[0]);
	assert(ret == 0);
	ret = thrd_join(threads[1]);
	assert(ret == 0);

	assert(count == 2);
	return 0;
}
