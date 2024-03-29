#include "librace.h" 
#include "model-assert.h"
#if defined(NIDHUGG)
#define smp_mb() asm volatile ("mfence" ::: "memory")
#elif defined(NIDHUGG_POWER)
#define smp_mb() asm volatile("sync" ::: "memory")
#else
#define smp_mb() atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst)
#endif

#ifdef MAKE_ALL_SC
# define access_mode memory_order_seq_cst
#else
# define access_mode memory_order_relaxed
#endif

bool latch1 = ATOMIC_VAR_INIT(true);
bool latch2 = ATOMIC_VAR_INIT(false);
bool flag1  = ATOMIC_VAR_INIT(true);
bool flag2  = ATOMIC_VAR_INIT(false);

void assume(int);

void *thread_1(void *unused)
{
	for (;;) {
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &latch1, access_mode));
		/* assert(!atomic_load_explicit(__FILE__, __LINE__, &latch1, access_mode) || */
		/*        atomic_load_explicit(__FILE__, __LINE__, &flag1, access_mode)); */

		atomic_store_explicit(__FILE__, __LINE__, &latch1, false, access_mode);
		if (atomic_load_explicit(__FILE__, __LINE__, &flag1, access_mode)) {
			atomic_store_explicit(__FILE__, __LINE__, &flag1, false, access_mode);
			atomic_store_explicit(__FILE__, __LINE__, &flag2, true, access_mode);
			smp_mb();
			atomic_store_explicit(__FILE__, __LINE__, &latch2, true, access_mode);
		}
	}
	;
}

void *thread_2(void *unused)
{
	for (;;) {
		__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &latch2, access_mode));
		/* assert(!atomic_load_explicit(__FILE__, __LINE__, &latch2, access_mode) || */
		/*        atomic_load_explicit(__FILE__, __LINE__, &flag2, access_mode)); */

		atomic_store_explicit(__FILE__, __LINE__, &latch2, false, access_mode);
		if (atomic_load_explicit(__FILE__, __LINE__, &flag2, access_mode)) {
			atomic_store_explicit(__FILE__, __LINE__, &flag2, false, access_mode);
			atomic_store_explicit(__FILE__, __LINE__, &flag1, true, access_mode);
			smp_mb();
			atomic_store_explicit(__FILE__, __LINE__, &latch1, true, access_mode);
		}
	}
	;
}
