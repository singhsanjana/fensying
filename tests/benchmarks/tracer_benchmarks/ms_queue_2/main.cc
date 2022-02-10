/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* The original benchmark is from CDSChecker */

/* Adapted to test with RCMC and TRACER */

#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#include "my_queue.h"
#include "model-assert.h"



#include "librace.h"

#define release memory_order_release
#define acquire memory_order_acquire
#define acq_rel memory_order_acq_rel

#define MAX_FREELIST 4 /* Each thread can own up to MAX_FREELIST free nodes */
#define INITIAL_FREE 2 /* Each thread starts with INITIAL_FREE free nodes */

#define POISON_IDX 0x666

#define LOOP 3

static unsigned int (*free_lists)[MAX_FREELIST];

/* Search this thread's free list for a "new" node */
static unsigned int new_node()
{
	int i;
	int t = get_thread_num();
	for (i = 0; i < MAX_FREELIST; i++) {
		unsigned int node = load_32(&free_lists[t][i]);
		if (node) {
			store_32(&free_lists[t][i], 0);
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
	MODEL_ASSERT(node);

	for (i = 0; i < MAX_FREELIST; i++) {
		/* Should never race with our own thread here */
		unsigned int idx = load_32(&free_lists[t][i]);

		/* Found empty spot in free list */
		if (idx == 0) {
			store_32(&free_lists[t][i], node);
			return;
		}
	}
	/* free list is full? */
	MODEL_ASSERT(0);
}

void init_queue(queue_t *q, int num_threads)
{
	int i, j;

	/* Initialize each thread's free list with INITIAL_FREE pointers */
	/* The actual nodes are initialized with poison indexes */
	free_lists = malloc(num_threads * sizeof(*free_lists));
	for (i = 0; i < num_threads; i++) {
		for (j = 0; j < INITIAL_FREE; j++) {
			free_lists[i][j] = 2 + i * MAX_FREELIST + j;
			atomic_init(&q->nodes[free_lists[i][j]].next, MAKE_POINTER(POISON_IDX, 0));
		}
	}

	/* initialize queue */
	atomic_init(&q->head, MAKE_POINTER(1, 0));
	atomic_init(&q->tail, MAKE_POINTER(1, 0));
	atomic_init(&q->nodes[1].next, MAKE_POINTER(0, 0));
}

void enqueue(queue_t *q, unsigned int val)
{
	int success = 0;
	unsigned int node;
	pointer tail;
	pointer next;
	pointer tmp;
  int OK = 0;
  
  node = new_node();
  store_32(&q->nodes[node].value, val);
  tmp = atomic_load_explicit(__FILE__, __LINE__, &q->nodes[node].next, acquire);
	set_ptr(&tmp, 0); // NULL
	atomic_store_explicit(__FILE__, __LINE__, &q->nodes[node].next, tmp, release);

 for (int i=0; i<LOOP; i++) {
  if (success) {
   OK = 1;
   break;
 } else {
   tail = atomic_load_explicit(__FILE__, __LINE__, &q->tail, acquire);
   next = atomic_load_explicit(__FILE__, __LINE__, &q->nodes[get_ptr(tail)].next, acquire);
   if (tail == atomic_load_explicit(__FILE__, __LINE__, &q->tail, acquire)) {
    
       	 			/* Check for uninitialized 'next' */
     MODEL_ASSERT(get_ptr(next) != POISON_IDX);
     
        			if (get_ptr(next) == 0) { // == NULL
                pointer value = MAKE_POINTER(node, get_count(next) + 1);
                success = atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &q->nodes[get_ptr(tail)].next,
                  &next, value, acq_rel, acq_rel);
              }
              if (!success) {
                unsigned int ptr = get_ptr(atomic_load_explicit(__FILE__, __LINE__, &q->nodes[get_ptr(tail)].next, acquire));
                pointer value = MAKE_POINTER(ptr, get_count(tail) + 1);
                atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &q->tail,
                  &tail, value,
                  acq_rel, acq_rel);
          				//thrd_yield();
              }
            }
          }
        }
        
        if (OK == 1)
          atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &q->tail,
            &tail,
            MAKE_POINTER(node, get_count(tail) + 1),
            acq_rel, acq_rel);
      }

      bool dequeue(queue_t *q, unsigned int *retVal)
      {
       int success = 0;
       int OK = 0;
       pointer head;
       pointer tail;
       pointer next;

       for (int i=0; i<LOOP; i++) {
        if (success) {
          OK = 1;
          break;
        } else {
          head = atomic_load_explicit(__FILE__, __LINE__, &q->head, acquire);
          tail = atomic_load_explicit(__FILE__, __LINE__, &q->tail, acquire);
          next = atomic_load_explicit(__FILE__, __LINE__, &q->nodes[get_ptr(head)].next, acquire);
          if (atomic_load_explicit(__FILE__, __LINE__, &q->head, acquire) == head) {
            if (get_ptr(head) == get_ptr(tail)) {
          /* Check for uninitialized 'next' */
              MODEL_ASSERT(get_ptr(next) != POISON_IDX);
              
          if (get_ptr(next) == 0) { // NULL
            return false; // NULL
          }
          atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &q->tail,
            &tail,
            MAKE_POINTER(get_ptr(next), get_count(tail) + 1),
            acq_rel, acq_rel);
          //thrd_yield();
        } else {
          *retVal = load_32(&q->nodes[get_ptr(next)].value);
          success = atomic_compare_exchange_strong_explicit(__FILE__, __LINE__, &q->head,
            &head,
            MAKE_POINTER(get_ptr(next), get_count(head) + 1),
            acq_rel, acq_rel);
          if (!success) {}
            //thrd_yield();
        }
    }
  }
}

if (OK == 0)
  return false;
else {
  reclaim(get_ptr(head));
  return true;
}
}




static int procs = 2;
static queue_t *queue;
static thrd_t *threads;
static unsigned int *input;
static unsigned int *output;
static int num_threads;

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

bool succ1, succ2;

static void main_task(void *param)
{
	unsigned int val;
	int pid = *((int *)param);
	if (!pid) {
		input[0] = 17;
		enqueue(queue, input[0]);
		succ1 = dequeue(queue, &output[0]);
	} else {
		input[pid] = 37;
		enqueue(queue, input[pid]);
		//enqueue(queue, input[pid]);
		//succ2 = dequeue(queue, &output[1]); //don't use this because race for RCMC?
	}
}

int user_main(int argc, char **argv)
{
	int i;
	int *param;
	unsigned int in_sum = 0, out_sum = 0;

	queue = calloc(1, sizeof(*queue));
	MODEL_ASSERT(queue);

	num_threads = procs;
	threads = malloc(num_threads * sizeof(thrd_t));
	param = malloc(num_threads * sizeof(*param));
	input = calloc(num_threads, sizeof(*input));
	output = calloc(num_threads, sizeof(*output));

	init_queue(queue, num_threads);
	for (i = 0; i < num_threads; i++) {
		param[i] = i;
		thrd_create(&threads[i], main_task, &param[i]);
	}
	

	return 0;
}
