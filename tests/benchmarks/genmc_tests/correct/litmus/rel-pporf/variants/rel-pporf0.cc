#include <stdatomic.h>
#include <threads.h>
#include "../rel-pporf.c"

int main()
{
	thrd_t t[2];

	pthread_create(&t[0], NULL, &runA, NULL);
	pthread_create(&t[1], NULL, &runB, NULL);

	return 0;
}
