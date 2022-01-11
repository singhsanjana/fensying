#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include <genmc.h>

int main()
{
	/* Make sure it's OK */
	free(NULL);

	return 0;
}
