#include "librace.h" 
#include "model-assert.h"
#include "twa_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
using namespace std;
#ifndef N
# define N 2
#endif

/* access the i-th array index */
#define TWA_ARRAY(i) &__twa_array[i].val

int shared;

static inline void twalock_init(struct twalock_s *l)
{
	atomic_init(&l->ticket, 0);
	atomic_init(&l->grant, 0);
}

static inline int wait_for_lock_slowpath(atomic_int *at, int u)
{
	int val;

	while ((val = atomic_load_explicit(__FILE__, __LINE__, at, memory_order_acquire)) == u)
		;
	return val;
}

static inline void twalock_acquire_slowpath(struct twalock_s *l, uint32_t t)
{
	int at = TWA_HASH(t);
	int u = atomic_load_explicit(__FILE__, __LINE__, TWA_ARRAY(at), memory_order_acquire);
	int k = atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_relaxed);
	int dx = TWA_DIFF(t, k);

	while (dx > TWA_L) {
		u = wait_for_lock_slowpath(TWA_ARRAY(at), u);
		dx = TWA_DIFF(t, atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_relaxed));
	}
}

static inline void twalock_acquire(struct twalock_s *l)
{
	int tx = atomic_fetch_add_explicit(__FILE__, __LINE__, &l->ticket, 1, memory_order_relaxed);
	int dx = TWA_DIFF(tx, atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_acquire));

	if (dx == 0)
		return;
	if (dx > TWA_L)
		twalock_acquire_slowpath(l, tx);

	while (atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_acquire) != tx)
		;
}

static inline void twalock_release(struct twalock_s *l)
{
	int k = atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_relaxed) + 1;
	atomic_store_explicit(__FILE__, __LINE__, &l->grant, k, memory_order_release);
	atomic_fetch_add_explicit(__FILE__, __LINE__, TWA_ARRAY(TWA_HASH(k + TWA_L)), 1, memory_order_release);
}

static inline int twalock_tryacquire(struct twalock_s *l)
{
	int k = atomic_load_explicit(__FILE__, __LINE__, &l->grant, memory_order_acquire);
	return atomic_compare_exchange_strong_explicit(__FILE__, __LINE__,&l->ticket, &k, k + 1,
						       memory_order_relaxed,
						       memory_order_relaxed) == k;
}


TWALOCK_ARRAY_DECL;
twalock_t lock = TWALOCK_INIT();

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	twalock_acquire(&lock);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	twalock_release(&lock);
	;
}

int user_main()
{
	thrd_t t[N];

	twalock_init(&lock);
	for (int i = 0u; i < N; i++)
		thrd_create(&t[i], (thrd_start_t)& thread_n, NULL);

	return 0;
}

