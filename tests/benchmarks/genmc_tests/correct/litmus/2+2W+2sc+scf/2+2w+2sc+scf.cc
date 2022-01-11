atomic_int x;
atomic_int y;

void *thread_1(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_seq_cst);
	return NULL;
}

void *thread_2(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_relaxed);
	return NULL;
}
