atomic_int x;
atomic_int y;

pthread_mutex_t l;

void *thread_1(void *ptr)
{
	for (int i = 0; i < 2; i++) {
		pthread_mutex_lock(&l);
		atomic_store_explicit(__FILE__, __LINE__, &x, 0, memory_order_release);
		pthread_mutex_unlock(&l);
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 0) {
			atomic_fetch_add_explicit(__FILE__, __LINE__, &y, 1, memory_order_acq_rel);
			atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
		}
	}
	return NULL;
}

void *thread_2(void *ptr)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 1) {
		if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) == 3) {
			/* assert(0); */
			/* printf("*****error*****\n"); */
			;
		} else {
			atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_release);
		}
	}
	return NULL;
}

void *thread_3(void *unused)
{
	for (int i = 0; i < 2; i++) {
		pthread_mutex_lock(&l);
		atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
		pthread_mutex_unlock(&l);
	}
	return NULL;
}
