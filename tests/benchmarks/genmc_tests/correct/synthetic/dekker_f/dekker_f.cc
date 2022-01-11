atomic_int x;
atomic_int y;
atomic_int c;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	if (!atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed)) {
		atomic_store_explicit(__FILE__, __LINE__, &c, 1, memory_order_relaxed);
		assert(atomic_load_explicit(__FILE__, __LINE__, &c, memory_order_relaxed) == 1);
	}
	return NULL;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	if (!atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed)) {
		atomic_store_explicit(__FILE__, __LINE__, &c, 0, memory_order_relaxed);
		assert(atomic_load_explicit(__FILE__, __LINE__, &c, memory_order_relaxed) == 0);
	}
	return NULL;
}
