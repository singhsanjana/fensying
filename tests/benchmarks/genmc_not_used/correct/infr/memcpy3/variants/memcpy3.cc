#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

struct bar {
	int x;
	int y;
};

struct foo {
	int b;
	struct bar c;
	int a[5];
};

int user_user_user_main()
{
	struct foo w = { .a = { [0 ... 2] = 42 }, .b = 42 };

	return 0;
}
