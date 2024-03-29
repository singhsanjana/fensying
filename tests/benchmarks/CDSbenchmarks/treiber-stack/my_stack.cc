#include <threads.h>
#include <stdlib.h>
#include "librace.h"
#include "model-assert.h"

#include "my_stack.h"

#define MAX_FREELIST 4 /* Each thread can own up to MAX_FREELIST free nodes */
#define INITIAL_FREE 2 /* Each thread starts with INITIAL_FREE free nodes */

#define POISON_IDX 0x666

#define LOOP 2

static unsigned int (*free_lists)[MAX_FREELIST];

/* Search this thread's free list for a "new" node */
static unsigned int new_node()
{
	int i;
	int t = get_thread_num();
	for (i = 0; i < MAX_FREELIST; i++) {
		//unsigned int node = load_32(&free_lists[t][i]);
		unsigned int node = free_lists[t][i];
		if (node) {
			//store_32(&free_lists[t][i], 0);
			free_lists[t][i] = 0;
			return node;
		}
	}
	/* free_list is empty? */
	MODEL_ASSERT(0);
	return 0;
}

/* Place this node index back on this thread's free list */
static void reclaim(unsigned int node)
{
	int i;
	int t = get_thread_num();

	/* Don't reclaim NULL node */
	//MODEL_ASSERT(node);

	for (i = 0; i < MAX_FREELIST; i++) {
		/* Should never race with our own thread here */
		//unsigned int idx = load_32(&free_lists[t][i]);
		unsigned int idx = free_lists[t][i];

		/* Found empty spot in free list */
		if (idx == 0) {
			//store_32(&free_lists[t][i], node);
			free_lists[t][i] = node;
			return;
		}
	}
	/* free list is full? */
	MODEL_ASSERT(0);
}

void init_stack(mystack_t *s, int num_threads)
{
	int i, j;

	/* Initialize each thread's free list with INITIAL_FREE pointers */
	/* The actual nodes are initialized with poison indexes */
	free_lists = (unsigned int (*)[4])malloc(num_threads * sizeof(*free_lists));
	for (i = 0; i < num_threads; i++) {
		for (j = 0; j < INITIAL_FREE; j++) {
			free_lists[i][j] = 1 + i * MAX_FREELIST + j;
			atomic_init(&s->nodes[free_lists[i][j]].next, MAKE_POINTER(POISON_IDX, 0));
		}
	}

	/* initialize stack */
	atomic_init(&s->top, MAKE_POINTER(0, 0));
}

void push(mystack_t *s, unsigned int val) {
	unsigned int nodeIdx = new_node();
	node_t *node = &s->nodes[nodeIdx];
	node->value = val;
	pointer oldTop, newTop;
	bool success;
	for (int loop = 0; loop < LOOP; loop++) {
		// acquire
		oldTop = atomic_load_explicit(__FILE__, __LINE__, &s->top, relaxed);
		newTop = MAKE_POINTER(nodeIdx, get_count(oldTop) + 1);
		// relaxed
		atomic_store_explicit(__FILE__, __LINE__, &node->next, oldTop, relaxed);

		// release & relaxed
		success = atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &s->top, &oldTop,
			newTop, relaxed, relaxed);
		if (success)
			break;
	} 
}

unsigned int pop(mystack_t *s) 
{
	pointer oldTop, newTop, next;
	node_t *node;
	bool success;
	int val;
	for (int loop = 0; loop < LOOP; loop++) {
		// acquire
		oldTop = atomic_load_explicit(__FILE__, __LINE__, &s->top, relaxed);
		if (get_ptr(oldTop) == 0)
			return 0;
		node = &s->nodes[get_ptr(oldTop)];
		// relaxed
		next = atomic_load_explicit(__FILE__, __LINE__, &node->next, relaxed);
		newTop = MAKE_POINTER(get_ptr(next), get_count(oldTop) + 1);
		// release & relaxed
		// success = atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &s->top, &oldTop,
		// 	newTop, relaxed, relaxed);
		pointer kkk = atomic_load_explicit(__FILE__, __LINE__, &s->top, relaxed);
		success = (kkk == oldTop);
		if (success) {
			atomic_store_explicit(__FILE__, __LINE__, &s->top, newTop, relaxed);
			break;
		} 
	}
	val = node->value;
	/* Reclaim the used slot */
	reclaim(get_ptr(oldTop));
	return val;
}
