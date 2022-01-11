_Atomic(atomic_int *) p;

int *x[2];

void *thread1(void *unused)
{
	p = malloc(sizeof(atomic_int));
	atomic_store_explicit(__FILE__, __LINE__, p, 42, memory_order_relaxed);
	return NULL;
}

void *thread2(void *unused)
{
	atomic_int *a = atomic_load_explicit(__FILE__, __LINE__, &p, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, a, memory_order_relaxed);
	return NULL;
}
