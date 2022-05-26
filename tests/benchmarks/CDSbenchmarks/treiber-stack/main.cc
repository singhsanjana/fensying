#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#include "my_stack.h"
#include "model-assert.h"

static int procs = 2;
static mystack_t *stack;
static thrd_t *threads;
static int num_threads;

unsigned int idx1, idx2;
unsigned int a, b;

atomic<int> s;

atomic_int x[3];

int get_thread_num()
{
	thrd_t curr = thrd_current();
	int i;
	for (i = 0; i < num_threads; i++)
		if (curr.priv == threads[i].priv)
			return i;
	MODEL_ASSERT(0);
	return -1;
}

static void main_task(void *param)
{
	unsigned int val;
	int pid = *((int *)param);
	fprintf(stderr, "pid=%d\n", pid);

	int l1, l2;
	l1 = l2 = 0;

	if (pid % 4 == 1) {
		unsigned int s_ = s.load(__FILE__, __LINE__);
		fprintf(stderr, "pid=%d, s_=%d\n", pid, s_);
		atomic_store_explicit(__FILE__, __LINE__, &x[s_], 17, relaxed);
		push(stack, s_);
		s += 1;
	} else if (pid % 4 == 2) {
		unsigned int s_ = s.load(__FILE__, __LINE__);
		fprintf(stderr, "pid=%d, s_=%d\n", pid, s_);
		atomic_store_explicit(__FILE__, __LINE__, &x[s_], 37, relaxed);
		push(stack, s_);
		s += 1;
	} else if (pid % 4 == 3) {
		// idx1 = pop(stack);
		// if (idx1 != 0) {
		// 	l1 = atomic_load_explicit(__FILE__, __LINE__, &x[idx1], relaxed);
		// }
	} else if (pid == 0) {
		idx2 = pop(stack);
		fprintf(stderr, "idx2=%d\n",idx2);
		if (idx2 == 2) {
			l1 = atomic_load_explicit(__FILE__, __LINE__, &x[idx2-1], relaxed);
			l2 = atomic_load_explicit(__FILE__, __LINE__, &x[idx2],   relaxed);
			MODEL_ASSERT( (l1==17 && l2==37) || (l1==37 && l2==17) );
		}
		else if (idx2 == 1) {
			l1 = atomic_load_explicit(__FILE__, __LINE__, &x[idx2], relaxed);
			fprintf(stderr, "l1=%d\n", l1);
			MODEL_ASSERT( l1==17 || l1==37 );
		}
	} else {}
}

int user_main(int argc, char **argv)
{
	int i;
	int *param;
	unsigned int in_sum = 0, out_sum = 0;

	atomic_init(&x[1], 0);
	atomic_init(&x[2], 0);
	atomic_init(&s, 1);
	idx1 = idx2 = a = b = 0;

	stack = (mystack_t*)calloc(1, sizeof(*stack));

	num_threads = procs;
	threads = (thrd_t*)malloc(num_threads * sizeof(thrd_t));
	param = (int*)malloc(num_threads * sizeof(*param));

	init_stack(stack, num_threads);
	printf("creating threads\n");
	
	for (i = 0; i < num_threads; i++) {
		param[i] = i;
		thrd_create(&threads[i], main_task, &param[i]);
	}
	for (i = 0; i < num_threads; i++)
		thrd_join(threads[i]);

	bool correct = false;
	//correct |= (a == 17 || a == 37 || a == 0);
	//MODEL_ASSERT(correct);

	free(param);
	free(threads);
	free(stack);
	
	return 0;
}
