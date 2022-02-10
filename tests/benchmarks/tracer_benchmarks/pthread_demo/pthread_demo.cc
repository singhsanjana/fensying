/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://github.com/sosy-lab/sv-benchmarks/blob/master/c/pthread-C-DAC/pthread-demo-datarace_true-unreach-call.c */





/***********************************************************************************
                         C-DAC Tech Workshop : hyPACK-2013
                             October 15-18,2013
  Example     : pthread-demo-datarace.c

  Objective   : Write Pthread code to illustrate Data Race Condition
            and its solution using MUTEX.

  Input       : Nothing.

  Output      : Value of Global variable with and without using Mutex.

  Created     :MAY-2013

  E-mail      : hpcfte@cdac.in

****************************************************************************/

/*
Modifications are made to remove non-standard libary dependencies by Yihao from
VSL of University of Delaware.
*/


#include <threads.h>
#include <stdatomic.h>
#include <mutex>
#include "librace.h"
#include "model-assert.h"

#define THREADNUM 2
#define LOOP 10

// shared variables
atomic_int myglobal; 
std::mutex *mymutex;

void  thread_function_mutex(void* arg)
{
  int i,j;
  for ( i=0; i<LOOP; i++ )
  {
    mymutex->lock();
    j=atomic_load_explicit(__FILE__, __LINE__, &myglobal, memory_order_acquire);
    j=j+1;
    atomic_store_explicit(__FILE__, __LINE__, &myglobal, j, memory_order_release);
    mymutex->unlock();
  }
}

int user_main(int argc, char **argv)
{
	thrd_t threads[THREADNUM];
	atomic_init(&myglobal, 0);
	mymutex =  new std::mutex();

	for (int i=0; i<THREADNUM; i++)
    thrd_create(&threads[i], thread_function_mutex, NULL);
  
  return 0;
}
