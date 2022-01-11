/* Full memory barrier */
#ifdef NIDHUGG
# define smp_mb() asm volatile ("mfence" ::: "memory")
#elif NIDHUGG_POWER
# define smp_mb() asm volatile ("sync" ::: "memory")
#else
# define smp_mb() atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst)
#endif

#ifndef N
# define N 1
#endif

atomic_int x;
atomic_int flag1;
atomic_int flag2;

void __VERIFIER_assume(int);

void *thread_1(void *unused)
{
	for (int i = 0u; i < N; i++) {
		atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_relaxed);
		smp_mb();
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) < 3);

		atomic_store_explicit(__FILE__, __LINE__, &flag1, 3, memory_order_relaxed);
		smp_mb();
		if (atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) == 1) {
			atomic_store_explicit(__FILE__, __LINE__, &flag1, 2, memory_order_relaxed);
			smp_mb();
			__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) == 4);
		}

		atomic_store_explicit(__FILE__, __LINE__, &flag1, 4, memory_order_relaxed);
		smp_mb();
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) < 2);

		/* Critical section start */
		atomic_store_explicit(__FILE__, __LINE__, &x, 0, memory_order_relaxed);
		atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
		assert(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) <= 0);
		smp_mb();
		/* Critical section end */

		__VERIFIER_assume(2 > atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) ||
				  atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed) > 3);
		atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_relaxed);
	}
	return NULL;
}

void *thread_2(void *unused)
{
	for (int i = 0u; i < N; i++) {
		atomic_store_explicit(__FILE__, __LINE__, &flag2, 1, memory_order_relaxed);
		smp_mb();
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) < 3);

		atomic_store_explicit(__FILE__, __LINE__, &flag2, 3, memory_order_relaxed);
		smp_mb();
		if (atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) == 1) {
			atomic_store_explicit(__FILE__, __LINE__, &flag2, 2, memory_order_relaxed);
			smp_mb();
			__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) == 4);
		}

		atomic_store_explicit(__FILE__, __LINE__, &flag2, 4, memory_order_relaxed);
		smp_mb();
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) < 2);

		/* Critical section start */
		atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
		assert(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) >= 1);
		smp_mb();
		/* Critical section end */

		__VERIFIER_assume(2 > atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) ||
				  atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed) > 3);
		atomic_store_explicit(__FILE__, __LINE__, &flag2, 0, memory_order_relaxed);
	}
	return NULL;
}
