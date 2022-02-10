/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from filesystem example in PLDI paper
   https://dl.acm.org/citation.cfm?id=1040315
 */

#include <atomic>
#include <threads.h>
#include <mutex>
#include "model-assert.h"

#include "librace.h"

#define NUMBLOCKS  26
#define NUMINODE  32

#define NUM_THREADS 19

#define LOOP 20

std::mutex  *locki[NUMINODE];
std::atomic<int> inode[NUMINODE];
std::mutex  *lockb[NUMBLOCKS];
std::atomic<bool> busy[NUMBLOCKS];

thrd_t  tids[NUM_THREADS];

// thread code
void thread_routine(void * arg)
{
  int tid;
  int i, b;
  tid = *((int *)arg);
  
  i = tid % NUMINODE;
  locki[i]->lock();
  if (inode[i].load(__FILE__, __LINE__, std::memory_order_acquire)==0) {
    b = (i*2) % NUMBLOCKS;
    for (int j=0; j<LOOP; j++) { // using loop instead of while
      lockb[b]->lock();
      if (!busy[b].load(__FILE__, __LINE__, std::memory_order_acquire)) {
        busy[b].store(__FILE__, __LINE__, true,std::memory_order_release);
        inode[i].store(__FILE__, __LINE__, b+1,std::memory_order_release);
        lockb[b]->unlock();
        break;
      }
      lockb[b]->unlock();
      b = (b+1) % NUMBLOCKS;
    }
  }
  locki[i]->unlock();

}


int user_main(int argc, char **argv)
{
  int i, arg[NUM_THREADS];

  // init
  for (i = 0; i < NUMINODE; i++) {
    locki[i] = new std::mutex();
    inode[i] = 0;
  }
  
  // init
  for (i = 0; i < NUMBLOCKS; i++) {
    lockb[i] = new std::mutex();
    busy[i] = false;
  }

  // create threads
  for (i = 0; i < NUM_THREADS; i++){
    arg[i] = i;
    thrd_create(&tids[i], thread_routine, &arg[i]);
  }


  for (i = 0; i < NUM_THREADS; i++){
    thrd_join(tids[i]);
  } 


  return 0;
}


