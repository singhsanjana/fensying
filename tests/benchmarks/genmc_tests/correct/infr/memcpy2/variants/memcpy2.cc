#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>

int main()
{
	char a[5] = "xxxx\0";

	return !(a[0] == 'x' && a[1] == 'x');
}
