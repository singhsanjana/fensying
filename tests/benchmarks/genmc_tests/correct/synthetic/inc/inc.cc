atomic_int x;
int idx[N];

void *thread_n(void *arg)
{
	int r, v = 1 << *((int *) arg);

	r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r + v, memory_order_release);
	return NULL;
}
