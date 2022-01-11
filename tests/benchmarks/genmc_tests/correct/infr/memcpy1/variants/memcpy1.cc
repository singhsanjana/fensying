#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

struct foo {
	int x;
	int y;
};

struct foo f;

int main()
{
	struct foo q = { .x = 42, .y = 42 };
	f = q;

	return !(f.x == 42 && f.x == 42);
}
