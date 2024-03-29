#include "librace.h" 
#include "model-assert.h"
#ifndef CONFIG_WRITERS
# define DEFAULT_WRITERS 1
#else
# define DEFAULT_WRITERS (CONFIG_WRITERS)
#endif

#define REPEAT10(x) x;x;x;x;x;x;x;x;x;x

atomic_int x;

void *thread_writer(void *unused)
{
#ifndef MANUAL_UNROLL
	for (int i = 0u; i < 42; i++)
		x = i;
#else
	REPEAT10(x = 1);
	REPEAT10(x = 1);
	REPEAT10(x = 1);
	REPEAT10(x = 1);
	x = 1;
	x = 1;
#endif
	;
}

void *thread_reader(void *unused)
{
	int a = x;
	while (a != 0)
		a = x;
	;
}
