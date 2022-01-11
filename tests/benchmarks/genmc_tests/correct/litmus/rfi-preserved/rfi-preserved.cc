atomic_int x;
atomic_int y;
atomic_int z;

void *thread_1(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);

	int b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &z, b, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *unused)
{
	int c = atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, c, memory_order_relaxed);
	return NULL;
}
