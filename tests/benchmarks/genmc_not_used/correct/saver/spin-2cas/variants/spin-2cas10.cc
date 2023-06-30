#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

atomic_int x;
atomic_int y;

int user_user_user_main()
{
	int r, b;

	r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	for (;;) {
		if (r == 42) {
			if (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
				continue;
		} else {
			if (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 17))
				continue;
		}
	}

	return 0;
}
