/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://github.com/sosy-lab/sv-benchmarks/blob/master/c/pthread/stack_true-unreach-call.c */

#include <threads.h>
#include <stdatomic.h>
#include <mutex>
#include "librace.h"
#include "model-assert.h"

#define TRUE      (1)
#define FALSE     (0)
#define SIZE      (12)
#define OVERFLOW  (-1)
#define UNDERFLOW (-2)
#define FULL (-3)
#define NUM_PUSH_THREADS 1
#define NUM_POP_THREADS 1

// shared variables
atomic_int top;
atomic_int  stack[SIZE];
std::mutex  *lock;

void inc_top(void)
{
  int _top;
  _top = atomic_load_explicit(__FILE__, __LINE__, &top, memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &top, _top+1, memory_order_release);
}

void dec_top(void)
{
  int _top;
  _top = atomic_load_explicit(__FILE__, __LINE__, &top, memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &top, _top-1, memory_order_release);
}

int get_top(void)
{
  int _top;
  _top = atomic_load_explicit(__FILE__, __LINE__, &top, memory_order_acquire);
  return _top;
}

int push(unsigned int x)
{
  if (get_top() > SIZE)
  {
    return OVERFLOW;
  } else if (get_top() == SIZE) { // full state of the stack
    return FULL;
  }
  else
  {
    atomic_store_explicit(__FILE__, __LINE__, &stack[get_top()], x, memory_order_release);
    inc_top();
  }
  return 0;
}

int pop(void)
{
  if (get_top()==0)
  {
    return UNDERFLOW;
  }
  else
  {
    dec_top();
    int _return = atomic_load_explicit(__FILE__, __LINE__, &stack[get_top()], memory_order_acquire);
    return _return;
  }
  return 0;
}


void pushthread(void *arg)
{
	int i, tid;
  unsigned int tmp;
  tid = *((int *)arg);

  for(i=0; i<SIZE; i++)
  {
    lock->lock();
    tmp = tid % SIZE;
    if (push(tmp) == OVERFLOW)
      MODEL_ASSERT(0);
    lock->unlock();
  }
  return;
}

void popthread(void *arg)
{
  int i, _top;

  for(i=0; i<SIZE; i++)
  {
    lock->lock();
    _top = atomic_load_explicit(__FILE__, __LINE__, &top, memory_order_acquire);
    if (_top > 0)
    {
      if (!(pop() != UNDERFLOW))
        MODEL_ASSERT(0);
    }
    lock->unlock();
  }
  return;
}

int user_main(int argc, char **argv)
{
	int i, arg[NUM_PUSH_THREADS];
	thrd_t t1s[NUM_PUSH_THREADS], t2s[NUM_POP_THREADS];

	atomic_init(&top, 0);
	lock = new std::mutex();

	for (int i=0; i<SIZE; i++)
    atomic_init(&stack[i], 0);

  for (int i=0; i<NUM_PUSH_THREADS; i++) {
    arg[i] = i;
    thrd_create(&t1s[i], pushthread, &arg[i]);
  }

  for (int i=0; i<NUM_POP_THREADS; i++) {
    thrd_create(&t2s[i], popthread, NULL);
  }

  return 0;
}
