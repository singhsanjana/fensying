#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 1
atomic_int x;

typedef struct qnode {
    struct qnode *next;
    atomic_int locked;
} qnode_t;

typedef struct qspinlock {
    atomic_uintptr_t tail;
    atomic_uintptr_t head;
} qspinlock_t;

qspinlock_t lock_object;

void qspinlock_init(qspinlock_t *lock) {
    atomic_init(&lock->head, NULL);
    atomic_init(&lock->tail, NULL);
}

int qspinlock_lock(qspinlock_t *lock, qnode_t *node) {
    node->next = NULL;
    atomic_store_explicit(__FILE__,__LINE__, &node->locked, 1, memory_order_seq_cst);

    qnode_t *prev = (qnode_t *)atomic_exchange_explicit(__FILE__,__LINE__, &lock->tail, (uintptr_t)node, memory_order_seq_cst);

    if (prev != NULL) {
        prev->next = node;
        for (int l = 0; l < LOOP; l++) {
            int locked = atomic_load_explicit(__FILE__,__LINE__, &node->locked, memory_order_seq_cst);
            if (locked == 0) {
                return 1;
            }
        }

        return 0;
    }

    atomic_store(__FILE__,__LINE__, &lock->head, (uintptr_t)node);
    return 1;
}

void qspinlock_unlock(qspinlock_t *lock, qnode_t *node) {
    qnode_t *next = node->next;
    if (next == NULL) {
        qnode_t *expected = node;
        if (atomic_compare_exchange_strong_explicit(__FILE__,__LINE__, &lock->head, (uintptr_t *)&expected, 0, memory_order_seq_cst, memory_order_seq_cst)) {
            return;
        }
        for (int l = 0; l < LOOP; l++) {
            next = node->next;
            if (next != NULL) {
                atomic_store_explicit(__FILE__,__LINE__, &next->locked, 0, memory_order_relaxed);
                return;
            }
        }

        return;
    }

    atomic_store_explicit(__FILE__,__LINE__, &next->locked, 0, memory_order_relaxed);
}

void fn1(void *arg) {
    qnode_t new_node;
    
    for (int i = 0; i < 2;  i++) {
        if (qspinlock_lock(&lock_object, &new_node)) {
            // Critical section
            atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(a==1);
            qspinlock_unlock(&lock_object, &new_node);
        }
    }
}

void fn2(void *arg) {
    qnode_t new_node;
    
    for (int i = 0; i < 2;  i++) {
        if (qspinlock_lock(&lock_object, &new_node)) {
            // Critical section
            atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(a==2);
            qspinlock_unlock(&lock_object, &new_node);
        }
    }
}

void fn3(void *arg) {
    qnode_t new_node;
    
    for (int i = 0; i < 2;  i++) {
        if (qspinlock_lock(&lock_object, &new_node)) {
            // Critical section
            atomic_store_explicit(__FILE__, __LINE__, &x, 3, memory_order_seq_cst);
            int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(a==3);
        }
        qspinlock_unlock(&lock_object, &new_node);
    }
}

int user_main(int argc, char **argv) {
    thrd_t tid1, tid2, tid3;
    atomic_init(&x, 0);

    qspinlock_init(&lock_object);
    
    thrd_create(&tid1, (thrd_start_t)fn1, NULL);
    thrd_create(&tid2, (thrd_start_t)fn2, NULL);
    // thrd_create(&tid3, (thrd_start_t)fn3, NULL);

    thrd_join(tid1);
    thrd_join(tid2);
    // thrd_join(tid3);

    return 0;
}