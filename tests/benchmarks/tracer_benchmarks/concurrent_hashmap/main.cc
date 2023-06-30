#include <threads.h>
#include "hashmap.h"

HashMap *table;




// Key(3, 2, 6) & Key(1, 3, 3) are hashed to the same slot -> 4
// Key(1, 1, 1) & Key(3, 2, 2) are hashed to the same slot -> 0
// Key(2, 4, 1) & Key(3, 4, 2) are hashed to the same slot -> 3
// Key(3, 4, 5) & Key(1, 4, 3) & Key(1, 1, 6) are hashed to the same slot -> 5
// Key(2, 4, 8) & Key(1, 3, 8) -> 9
// Key(1, 4, 8) -> 10
// Key(1, 3, 7) -> 8
// Key(1, 2, 7) -> 7
// Key(1, 2, 6) -> 6


void threadA(void *arg) {
	Key *k1 = new Key(3, 2, 6);
	Key *k2 = new Key(1, 1, 1);
	Value *v1 = new Value(10, 10, 10);
	Value *r1 = table->put(k1, v1);
	Value *r2 = table->get(k2);
}

void threadB(void *arg) {
	Key *k1 = new Key(3, 2, 6);
	Key *k2 = new Key(1, 1, 1);
	Value *v2 = new Value(30, 40, 50);
	Value *r3 = table->put(k2, v2);
	Value *r4 = table->get(k1);
}


void threadC(void *arg) {
	Key *k1 = new Key(3, 4, 5);
	Key *k2 = new Key(1, 4, 3);
	Key *k3 = new Key(1, 1, 6);
	Value *v2 = new Value(10, 10, 10);
	Value *r1 = table->put(k2, v2);
	Value *r2 = table->get(k3);
}

void threadD(void *arg) {
	Key *k1 = new Key(3, 4, 5);
	Key *k2 = new Key(1, 4, 3);
	Key *k3 = new Key(1, 1, 6);
	Value *v3 = new Value(30, 40, 50);
	Value *r3 = table->put(k3, v3);
	Value *r4 = table->get(k1);
}


int user_main(int argc, char *argv[]) {
	thrd_t t1, t2, t3, t4;

	table = new HashMap;

	thrd_create(&t1, threadA, NULL);
	thrd_create(&t2, threadB, NULL);
	thrd_create(&t3, threadC, NULL);
	thrd_create(&t4, threadD, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);
	
	return 0;
}


