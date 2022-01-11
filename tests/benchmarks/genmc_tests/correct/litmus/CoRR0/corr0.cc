atomic_int x;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	return NULL;
}

void *thread_2(void *unused)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	return NULL;
}

void *thread_3(void *unused)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	return NULL;
}
