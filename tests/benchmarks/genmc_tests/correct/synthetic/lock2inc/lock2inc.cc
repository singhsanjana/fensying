pthread_mutex_t mutex;
atomic_int x;
atomic_int y;

void *thread_n(void *unused)
{
	int r1, r2;

	pthread_mutex_lock(&mutex);
	r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r1 + 1, memory_order_relaxed);
	r2 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, r2 + 1, memory_order_relaxed);
	pthread_mutex_unlock(&mutex);
	return NULL;
}
