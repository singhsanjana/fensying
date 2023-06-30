#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 1
#define INNER_LOOP 1
typedef struct {
    atomic_int locked;
} QSpinLock;

void QSpinLock_init(QSpinLock* lock) {
    // lock->locked = false;
    atomic_init(&lock->locked, 0);
}

void QSpinLock_lock(QSpinLock* lock) {
    for (int i=0;i<LOOP;i++)
    {
        // int a = atomic_load_explicit(__FILE__,__LINE__,&lock->locked,memory_order_relaxed);
        // atomic_store_explicit(__FILE__,__LINE__,&lock->locked, 1, memory_order_relaxed);
        // if(a==0){
        //     return;
        // }
        if(atomic_exchange_explicit(__FILE__,__LINE__,&lock->locked, 1,memory_order_relaxed)==0){
            return;
        }
        for (int j=0;j<INNER_LOOP;j++)
        {
            if(atomic_load_explicit(__FILE__,__LINE__,&lock->locked,memory_order_relaxed)==0){
                break;
            }
        }
        
    }
    
}

void QSpinLock_unlock(QSpinLock* lock) {
    atomic_store_explicit(__FILE__,__LINE__,&lock->locked, 0,memory_order_relaxed);
}

QSpinLock lock;
atomic_int  sd1;
// atomic_int  sd2;
void tf1(void *arg) {
  QSpinLock_lock(&lock);
  // Critical section
  for(int i=0;i<2;i++){
    atomic_store_explicit(__FILE__,__LINE__,&sd1,10,memory_order_relaxed);
    int a = atomic_load_explicit(__FILE__,__LINE__,&sd1, memory_order_relaxed);
    MODEL_ASSERT(a==10);
    QSpinLock_unlock(&lock);
  }
}

void tf2(void *arg){
    for(int i=0;i<2;i++){
        QSpinLock_lock(&lock);
        atomic_store_explicit(__FILE__,__LINE__,&sd1, 20, memory_order_relaxed);
        int b = atomic_load_explicit(__FILE__,__LINE__,&sd1, memory_order_relaxed);
        MODEL_ASSERT(b==20);
        QSpinLock_unlock(&lock);
    }
}



int user_main(int argc, char **argv) {
  thrd_t tid1, tid2;
  atomic_init(&sd1,1);  
//   atomic_init(&sd2,1);  
  QSpinLock_init(&lock);
  thrd_create(&tid1, (thrd_start_t)tf1, NULL);
  thrd_create(&tid2, (thrd_start_t)tf2, NULL);

  thrd_join(tid1);
  thrd_join(tid2);

  return 0;
}
