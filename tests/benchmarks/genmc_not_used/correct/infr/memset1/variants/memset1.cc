#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

struct foo {
	int a[8];
	int x;
	long y;
	char z;
};

struct foo f;

int user_user_main()
{
	struct foo q = { .x = 42 };

	return 0;
}
