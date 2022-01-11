atomic_int x;

void *thread_1(void *unused)
{
	while (1) {
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) == 42) {
			continue;
		} else {
			atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_relaxed);
			continue;
		}
	}
	return NULL;
}

void *thread_2(void *unused)
{
	for (int i = 0u; i < 2; i++)
		x = i;
	return NULL;
}
