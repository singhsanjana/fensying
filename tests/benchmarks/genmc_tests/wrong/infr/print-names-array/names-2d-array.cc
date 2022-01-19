#include "librace.h" 
#include "model-assert.h"
typedef struct node {
	int value;
	int next;

} node_t;

typedef struct {
	int top;
	node_t nodes[4][2];
} mystack_t;

mystack_t stack;

atomic_int x;

int a[2][2];

void *thread_1(void *unused)
{
	stack.nodes[2][1].next = 2;
	stack.nodes[3][1].value = 3;

	if (x.load(__FILE__, __LINE__) == 42) {
		a[1][1] = 42;
		a[0][0] = 17;
	}
	;
}

void *thread_2(void *unused)
{
	a[1][0] = 3;
	stack.nodes[1][1].value = 42;
	x = 42;
	;
}
