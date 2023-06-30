#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define kMaxThreads 100

typedef struct {
  atomic_bool flags[kMaxThreads];
  atomic_int tickets[kMaxThreads];
  atomic_int turn;
} TWALock;

void TWALock_init(TWALock* lock) {
  for (int i = 0; i < kMaxThreads; i++) {
    atomic_init(&lock->flags[i], false);
    atomic_init(&lock->tickets[i], 0);
  }
  atomic_init(&lock->turn, 0);
}

void TWALock_lock(TWALock* lock, int id) {
  int my_ticket = atomic_fetch_add(&lock->tickets[id], 1);
  atomic_store(&lock->flags[id], true);
  int max_ticket = atomic_load(&lock->turn);
  while (my_ticket != max_ticket) {
    max_ticket = atomic_load(&lock->turn);
  }
}

void TWALock_unlock(TWALock* lock, int id) {
  atomic_store(&lock->flags[id], false);
  atomic_store(&lock->turn, id + 1);
}

TWALock lock;
atomic_int sd1;
int tf1(void* arg) {
  int thread_id = *(int*) arg;
  for (int i=0;i<2;i++){
    TWALock_lock(&lock, thread_id);
    atomic_store_explicit(&sd1, 10, memory_order_relaxed);
    int a = atomic_load_explicit(&sd1, memory_order_relaxed);
    MODEL_ASSERT(a==10);
    // Critical section
    TWALock_unlock(&lock, thread_id);
  }
  return 0;
}
int tf1(void* arg) {
  int thread_id = *(int*) arg;
  for (int i=0;i<2;i++){
    TWALock_lock(&lock, thread_id);
    atomic_store_explicit(&sd1, 20, memory_order_relaxed);
    int b = atomic_load_explicit(&sd1, memory_order_relaxed);
    MODEL_ASSERT(b==20);
    // Critical section
    TWALock_unlock(&lock, thread_id);
  }
  return 0;
}

int user_main(int argc, char **argv) {
  int num_threads = 2;
  thrd_t threads[num_threads];
  int thread_ids[num_threads];
  // for (int i = 0; i < num_threads; i++) {
    // thread_ids[i] = i;
  thread_ids[0] = 0;
  thread_ids[1] = 1;
  thrd_create(&threads[0], tf1, &thread_ids[0]);
  thrd_create(&threads[1], tf1, &thread_ids[1]);
  // }
  for (int i = 0; i < num_threads; i++) {
    thrd_join(threads[i]);
  }
  return 0;
}
