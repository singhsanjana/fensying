int x;
atomic_int y;

void *thread_1(void *arg)
{
	x = 1;
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *arg)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire))
		x = 2;
	return NULL;
}
