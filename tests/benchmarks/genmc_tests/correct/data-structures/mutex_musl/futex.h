#include "librace.h" 
#include "model-assert.h"
#ifndef __FUTEX_H__
#define __FUTEX_H__

static atomic_int signal;

static inline void __futex_wait(atomic_int *m, int v)
{
	int s = atomic_load_explicit(__FILE__, __LINE__, &signal, memory_order_acquire);
	if (atomic_load_explicit(__FILE__, __LINE__, m, memory_order_acquire) != v)
		return;

	while (atomic_load_explicit(__FILE__, __LINE__, &signal, memory_order_acquire) == s)
		;
}

static inline void __futex_wake(atomic_int *m, int v)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &signal, 1, memory_order_release);
}

#endif /* __FUTEX_H__ */
