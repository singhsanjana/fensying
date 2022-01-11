atomic_int a;
atomic_int b;
atomic_int c;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &a, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &b, 1, memory_order_relaxed);
	return NULL;
}

void *thread_2(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, &c, 1, memory_order_relaxed);
	return NULL;
}

void *thread_3(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &c, 2, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_relaxed);
	return NULL;
}
