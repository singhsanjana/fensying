#include "librace.h" 
#include "model-assert.h"
#define THREADS 2

struct Node {
	struct Node* next;
};

_Atomic(struct Node*) list;

thrd_t t[THREADS];
struct Node n[THREADS];

void* producer_thread(void* param)
{
	struct Node* node;

	node =  (struct Node*)param;

	// Insert node at beginning of the list.
	node->next = atomic_load(&list);
	while (!atomic_compare_exchange_weak(__FILE__, __LINE__, &list, &node->next, node))
		;

	;
}
