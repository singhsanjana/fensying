atomic_int x = ATOMIC_VAR_INIT(42);

void *thread_1(void *unused)
{
	for (;;) {
		atomic_fetch_sub_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) == 42);
	}
	return NULL;
}

void *thread_2(void *unused)
{
	for (;;) {
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, -1, memory_order_relaxed);
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) == 42);
	}
	return NULL;
}
