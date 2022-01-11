void __VERIFIER_assume(int x);

atomic_int x;
atomic_int y;
atomic_int z;

_Atomic(atomic_int *) p;

void *thread_zero(void *unused)
{
	atomic_store_explicit(__LINE__, &x, 3, memory_order_release);
	atomic_store_explicit(__LINE__, &y, 4, memory_order_release);
	atomic_store_explicit(__LINE__, &z, 1, memory_order_release);
	return NULL;
}

void *thread_one(void *unused)
{
	int c1 = 0, init;
	atomic_int * t;

	init = atomic_load_explicit(__LINE__, &z, memory_order_acquire);
	__VERIFIER_assume(init == 1);
	atomic_store_explicit(__LINE__, &p, &y, memory_order_release);
	for (int i = 0; i < N; i++)
		c1 += atomic_load_explicit(__LINE__, &x, memory_order_acquire);
	t = atomic_load_explicit(__LINE__, &p, memory_order_acquire);
	atomic_store_explicit(t, atomic_load_explicit(__LINE__, t, memory_order_acquire) + 3,
			      memory_order_release);
	/* MODEL_ASSERT(3 <= x && x <= 9); */
	/* MODEL_ASSERT(3 <= y && y <= 9); */
	return NULL;
}

void *thread_two(void *unused)
{
	int c2 = 0, init;
	atomic_int * t;

	init = atomic_load_explicit(__LINE__, &z, memory_order_acquire);
	__VERIFIER_assume(init == 1);
	atomic_store_explicit(__LINE__, &p, &x, memory_order_release);
	for (int i = 0; i < N; i++)
		c2 += atomic_load_explicit(__LINE__, &y, memory_order_acquire);
	t = atomic_load_explicit(__LINE__, &p, memory_order_acquire);
	atomic_store_explicit(t, atomic_load_explicit(__LINE__, t, memory_order_acquire) + 3,
			      memory_order_release);
	/* MODEL_ASSERT(3 <= x && x <= 9); */
	/* MODEL_ASSERT(3 <= y && y <= 9); */
	return NULL;
}
