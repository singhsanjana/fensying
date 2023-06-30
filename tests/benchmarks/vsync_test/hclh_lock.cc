#include <stdio.h>
#include <threads.h>
#include "librace.h"
#include "model-assert.h"
#include <atomic>

class HCLHLock {
 private:
  struct HCLHNode {
    bool is_locked;
    HCLHNode *next;
  };

  std::atomic<HCLHNode *> tail_;

 public:
  HCLHLock() {
    HCLHNode *node = new HCLHNode();
    node->is_locked = false;
    node->next = nullptr;
    tail_.store(node);
  }

  void lock() {
    HCLHNode *node = new HCLHNode();
    node->is_locked = true;
    node->next = nullptr;
    HCLHNode *prev = tail_.exchange(node);
    while (prev->is_locked) {
      // spin
    }
  }

  void unlock() {
    HCLHNode *node = tail_.load();
    node->is_locked = false;
    tail_.store(node->next);
  }
};

HCLHLock lock;
int shared_counter = 0;

void increment_counter(void *arg) {
  for (int i = 0; i < 100000; i++) {
    lock.lock();
    shared_counter++;
    lock.unlock();
  }
  // return nullptr;
}

int user_main(int argc, char **argv) {
  thrd_t threads[10];
  for (int i = 0; i < 10; i++) {
    thrd_create(&threads[i], (thrd_start_t)&increment_counter, nullptr);
  }
  for (int i = 0; i < 10; i++) {
    thrd_join(threads[i]);
  }
  return 0;
}
