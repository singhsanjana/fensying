#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

/*
 * If we change the label "exit" to "body", this loop does not qualify
 * as a natural loop and does not terminate.
 * We currently do not bound such loops, so even using unroll would be useless.
 */

atomic_int x;

int user_user_user_main()
{
	/* Depending on the value read, the loop has a different entry point */
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed))
		goto exit; /* replace w/ body */

	while (x != 42) {
body:
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) + 17 == 42)
			break;
	}

exit:
	return 0;
}
