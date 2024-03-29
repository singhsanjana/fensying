#include "librace.h" 
#include "model-assert.h"
bool flag1; /* Boolean flags */
bool flag2;

atomic_int turn;   /* Atomic integer that holds the ID of the thread whose turn it is */
bool x;     /* Boolean variable to test mutual exclusion */

void assume(int);

void *thread_1(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &turn, 1, memory_order_seq_cst);

	__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_seq_cst) != 1 ||
			  atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_seq_cst) != 1);

	/* critical section beginning */
	atomic_store_explicit(__FILE__, __LINE__, &x, 0, memory_order_seq_cst);
//	assert(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst) <= 0);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	/* critical section ending */

	atomic_store_explicit(__FILE__, __LINE__, &flag1, 0, memory_order_seq_cst);
	;
}

void *thread_2(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &flag2, 1, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &turn, 0, memory_order_seq_cst);

	__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_seq_cst) != 1 ||
			  atomic_load_explicit(__FILE__, __LINE__, &turn, memory_order_seq_cst) != 0);

	/* critical section beginning */
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
//	assert(atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst) >= 1);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	/* critical section ending */

	atomic_store_explicit(__FILE__, __LINE__, &flag2, 0, memory_order_seq_cst);
	;
}
