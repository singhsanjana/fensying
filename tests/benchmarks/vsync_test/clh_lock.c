#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

#define CACHE_LINE_SIZE 64
#define LOOP 1
#define MAX_THREADS 2

atomic_int x;

typedef struct clh_node {
    atomic_int locked;
    struct clh_node *prev;
} clh_node_t;

typedef _Atomic clh_node_t* atomic_ptr;

typedef struct clh_lock {
    atomic_uintptr_t head;
    atomic_uintptr_t tail;
} clh_lock_t;

void clh_node_init(clh_node_t *node) {
    // node = malloc(sizeof(clh_node_t));
    atomic_store_explicit(__FILE__, __LINE__, &node->locked, 0, memory_order_seq_cst);
    node->prev = NULL;
}

void clh_lock_init(clh_lock_t *lock) {
    clh_node_t node;
    clh_node_init(&node);
    atomic_init(&lock->head, (uintptr_t)&node);
    atomic_init(&lock->tail, (uintptr_t)&node);
    // atomic_store_explicit(__FILE__,__LINE__, &lock->tail, (uintptr_t)node, memory_order_seq_cst);
    // atomic_init(&lock->head, NULL);
    // atomic_init(&lock->tail, NULL);
}

int clh_lock_lock(clh_lock_t *lock, clh_node_t *node) {
    atomic_store_explicit(__FILE__, __LINE__, &node->locked, 1, memory_order_relaxed);
    node->prev = (clh_node_t *)atomic_exchange_explicit(__FILE__,__LINE__, &lock->tail, (uintptr_t)node, memory_order_acq_rel);
    
    for (int l = 0; l < LOOP; l++) {
        int prev_locked = atomic_load_explicit(__FILE__, __LINE__, &node->prev->locked, memory_order_relaxed);
        if (!prev_locked) {
            return 1;
        }
    }

    return 0;
}

void clh_lock_unlock(clh_lock_t *lock, clh_node_t *node) {
    atomic_store_explicit(__FILE__, __LINE__, &node->locked, 0, memory_order_relaxed);
    // node = (clh_node_t *)atomic_load_explicit(__FILE__,__LINE__, &lock->tail, memory_order_seq_cst);
    // int prev_locked = atomic_load_explicit(__FILE__, __LINE__, &node->prev->locked, memory_order_seq_cst);
    // if (!prev_locked) {
    //     atomic_store_explicit(__FILE__, __LINE__, &lock->tail, (uintptr_t)node->prev, memory_order_seq_cst);
    //     free(node);
    // }
    // else {
    //     atomic_store_explicit(__FILE__, __LINE__, &node->locked, 0, memory_order_seq_cst);
    // }
}

clh_lock_t my_lock;

void my_thread_func(void *arg) {
    int thread_id = *(int*)arg;

    clh_node_t my_node;
    clh_node_init(&my_node);

    for (int i = 0; i < LOOP; i++) {
        if (clh_lock_lock(&my_lock, &my_node)) {
            atomic_store_explicit(__FILE__, __LINE__, &x, thread_id+1, memory_order_seq_cst);
            int v = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
            MODEL_ASSERT(v == thread_id+1);
            clh_lock_unlock(&my_lock, &my_node);
        }
    }
}

int user_main(int argc, char **argv) {
    clh_lock_init(&my_lock);

    thrd_t threads[2];
    int thread_args[2];
    
    atomic_init(&x, 0);
    
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_args[i] = i;
        thrd_create(&threads[i], my_thread_func, &thread_args[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        thrd_join(threads[i]);
    }

    return 0;
}
