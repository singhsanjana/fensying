#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "../main.cc"

int user_user_user_main()
{
	/* Store PIDs starting from the first entry of threads[] */
	int i = 1;

	init();
	for (int j = 0; j < adders; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& thread_add, NULL);
	for (int j = 0; j < seekers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& thread_seek, NULL);
	for (int j = 0; j < removers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& thread_del, NULL);

	/* i = 1; */
	/* for (int j = 0; j < adders; j++, i++) */
	/* 	pthread_join(threads[i], NULL); */
	/* for (int j = 0; j < seekers; j++, i++) */
	/* 	pthread_join(threads[i], NULL); */
	/* for (int j = 0; j < removers; j++, i++) */
	/* 	pthread_join(threads[i], NULL); */

	/* traverse(&mybst); */
	/* printf("---\n"); */

	return 0;
}
