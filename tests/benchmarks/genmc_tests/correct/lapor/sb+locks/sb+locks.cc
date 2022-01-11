pthread_mutex_t l;

atomic_int x;
atomic_int y;

int r_x;
int r_y;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);

	pthread_mutex_lock(&l);
	pthread_mutex_unlock(&l);

	r_x = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);

	pthread_mutex_lock(&l);
	pthread_mutex_unlock(&l);

	r_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	return NULL;
}
