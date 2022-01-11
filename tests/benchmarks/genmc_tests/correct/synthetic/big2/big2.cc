atomic_int x;
atomic_int y;
atomic_int z;
atomic_int w;

void *thread_one(void *arg)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	return NULL;
}

void *thread_two(void *arg)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	return NULL;
}

void *thread_three(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &z, 1, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &z, 2, memory_order_release);
	return NULL;
}

void *thread_four(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 3, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &y, 3, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &z, 3, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &x, 4, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &y, 4, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &z, 4, memory_order_release);
	return NULL;
}
