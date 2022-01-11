atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *unused)
{
	int r = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	return NULL;
}
