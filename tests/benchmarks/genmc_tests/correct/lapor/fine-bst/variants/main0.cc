#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include "../main.c"

int main()
{
	/* Store PIDs starting from the first entry of threads[] */
	int i = 1;

	init();
	for (int j = 0; j < adders; j++, i++)
		pthread_create(&threads[i], NULL, thread_add, &param[i]);
	for (int j = 0; j < seekers; j++, i++)
		pthread_create(&threads[i], NULL, thread_seek, &param[i]);
	for (int j = 0; j < removers; j++, i++)
		pthread_create(&threads[i], NULL, thread_del, &param[i]);

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
