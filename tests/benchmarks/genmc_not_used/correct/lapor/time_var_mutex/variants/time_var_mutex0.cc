#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../time_var_mutex.cc"

int user_user_user_main()
{
	thrd_t t1, t2;

	assume(inode == busy);

	if (pthread_mutex_init(&m_inode, NULL))
		MODEL_ASSERT(0);
	if (pthread_mutex_init(&m_busy, NULL))
		MODEL_ASSERT(0);

	if (thrd_create(&t1, (thrd_start_t)& allocator, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& de_allocator, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	/* pthread_mutex_destroy(&m_inode); */
	/* pthread_mutex_destroy(&m_busy); */
	return 0;
}
