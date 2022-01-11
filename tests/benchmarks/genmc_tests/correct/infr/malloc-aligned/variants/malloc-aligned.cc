#include <stdlib.h>
#include <inttypes.h>
#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

struct foo {
	int a;
	int b;
	int c;
} __attribute__((__aligned__(8)));

/* This char will trick older versions of GenMC and make the assert()
 * fire because malloc() did not return a properly-aligned address */
char a;

int main()
{
	void *a = malloc(sizeof(struct foo));
	/* printf("addr: %p\n", a); */

	/* should not fire */
	assert(!(((uintptr_t)a & 7)));

	return 0;
}
