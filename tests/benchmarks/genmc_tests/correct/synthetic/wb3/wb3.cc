#define mo_rlx memory_order_relaxed

atomic_int x;

void __VERIFIER_assume(int);

void *thread1(void *arg)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	__VERIFIER_assume(a == 6);
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	return NULL;
}

void *thread2(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 4, mo_rlx);
	atomic_store_explicit(__FILE__, __LINE__, &x, 6, mo_rlx);
	return NULL;
}

void *thread3(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 3, mo_rlx);
	int d = atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, mo_rlx);
	__VERIFIER_assume(d == 4);
	return NULL;
}

void *thread4(void *arg)
{
	int e = atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 10, mo_rlx);
	__VERIFIER_assume(e == 3);
	return NULL;
}
