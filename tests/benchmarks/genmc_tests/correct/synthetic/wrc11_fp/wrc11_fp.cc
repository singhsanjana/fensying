atomic_int x;
int a, b;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	return NULL;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	b = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	return NULL;
}
