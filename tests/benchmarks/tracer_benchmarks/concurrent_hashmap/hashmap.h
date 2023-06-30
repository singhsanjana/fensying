#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <atomic>
#include "stdio.h" 
//#include <common.h>
#include <stdlib.h>
#include <mutex>

//#include "sc_annotation.h"

#define relaxed memory_order_relaxed
#define release memory_order_release
#define acquire memory_order_acquire
#define acq_rel memory_order_acq_rel
#define seq_cst memory_order_seq_cst

using namespace std;

/**
	For the sake of simplicity, we do not use template but some toy structs to
	represent the Key and Value.
*/
struct Key {
	// Probably represent the coordinate (x, y, z)
	int x;
	int y;
	int z;

	int hashCode() {
		return x + 31 * y + 31 * 31 * z;
	}

	bool equals(Key *other) {
		if (!other)
			return false;
		return x == other->x && y == other->y && z == other->z;
	}

	Key(int x, int y, int z) :
		x(x),
		y(y),
		z(z)
	{

	}
};

struct Value {
	// Probably represent the speed vector (vX, vY, vZ)
	int vX;
	int vY;
	int vZ;

	Value(int vX, int vY, int vZ) :
		vX(vX),
		vY(vY),
		vZ(vZ)
	{

	}

	bool equals(Value *other) {
		if (!other)
			return false;
		return vX == other->vX && vY == other->vY && vZ == other->vZ;
	}
};

class Entry {
	public:
	Key *key;
	atomic<Value*> value;
	int hash;
	atomic<Entry*> next;

	Entry(int h, Key *k, Value *v, Entry *n) {
		this->hash = h;
		this->key = k;
		this->value.store(__FILE__, __LINE__, v, release);
		this->next.store(__FILE__, __LINE__, n, release);
	}
};

class Segment {
	public:
	int count;
	mutex segMutex;

	void lock() {
		segMutex.lock();
	}

	void unlock() {
		segMutex.unlock();
	}

	Segment() {
		this->count = 0;
	}
};

class HashMap {
	public:
	atomic<Entry*> *table;

	int capacity;
	int size;

	static const int CONCURRENCY_LEVEL = 16;

	static const int SEGMENT_MASK = CONCURRENCY_LEVEL - 1;

	Segment *segments[CONCURRENCY_LEVEL];

	static const int DEFAULT_INITIAL_CAPACITY = 16;

	// Not gonna consider resize now...
	
	HashMap() {
		this->size = 0;
		this->capacity = DEFAULT_INITIAL_CAPACITY;
		this->table = new atomic<Entry*>[capacity];
		for (int i = 0; i < capacity; i++) {
			atomic_init(&table[i], NULL);
		}
		for (int i = 0; i < CONCURRENCY_LEVEL; i++) {
			segments[i] = new Segment;
		}
	}

	int hashKey(Key *x) {
		int h = x->hashCode();
		// Use logical right shift
		unsigned int tmp = (unsigned int) h;
		return ((h << 7) - h + (tmp >> 9) + (tmp >> 17));
	}

	bool eq(Key *x, Key *y) {
		return x == y || x->equals(y);
	}

	Value* get(Key *key) {
		//MODEL_ASSERT (key);
		int hash = hashKey(key);

		// Try first without locking...
		atomic<Entry*> *tab = table;
		int index = hash & (capacity - 1);
		atomic<Entry*> *first = &tab[index];
		Entry *e;
		Value *res = NULL;

		// Should be a load acquire
		// This load action here makes it problematic for the SC analysis, what
		// we need to do is as follows: if the get() method ever acquires the
		// lock, we ignore this operation for the SC analysis, and otherwise we
		// take it into consideration
		
		//SC_BEGIN();
		Entry *firstPtr = first->load(__FILE__, __LINE__, acquire);
		//SC_END();

		e = firstPtr;
		while (e != NULL) {
			if (e->hash == hash && eq(key, e->key)) {
				res = e->value.load(__FILE__, __LINE__, acquire);
				if (res != NULL)
					return res;
				else
					break;
			}
			// Loading the next entry, this can be relaxed because the
			// synchronization has been established by the load of head
			e = e->next.load(__FILE__, __LINE__, acquire);
		}
	
		// Recheck under synch if key apparently not there or interference
		Segment *seg = segments[hash & SEGMENT_MASK];
		seg->lock(); // Critical region begins
		// Not considering resize now, so ignore the reload of table...

		// Synchronized by locking, no need to be load acquire
		Entry *newFirstPtr = first->load(__FILE__, __LINE__, acquire);
		if (e != NULL || firstPtr != newFirstPtr) {
			e = newFirstPtr;
			while (e != NULL) {
				if (e->hash == hash && eq(key, e->key)) {
					// Protected by lock, no need to be SC
					res = e->value.load(__FILE__, __LINE__, acquire);
					seg->unlock(); // Critical region ends
					return res;
				}
				// Synchronized by locking
				e = e->next.load(__FILE__, __LINE__, acquire);
			}
		}
		seg->unlock(); // Critical region ends
		return NULL;
	}

	Value* put(Key *key, Value *value) {
		// Don't allow NULL key or value
		//MODEL_ASSERT (key && value);

		int hash = hashKey(key);
		Segment *seg = segments[hash & SEGMENT_MASK];
		atomic<Entry*> *tab;

		seg->lock(); // Critical region begins
		tab = table;
		int index = hash & (capacity - 1);

		atomic<Entry*> *first = &tab[index];
		Entry *e;
		Value *oldValue = NULL;
	
		// The written of the entry is synchronized by locking
		Entry *firstPtr = first->load(__FILE__, __LINE__, acquire);
		e = firstPtr;
		while (e != NULL) {
			if (e->hash == hash && eq(key, e->key)) {
				// FIXME: This could be a relaxed (because locking synchronize
				// with the previous put())??  no need to be acquire
				oldValue = e->value.load(__FILE__, __LINE__, acquire);
				e->value.store(__FILE__, __LINE__, value, release);
				seg->unlock(); // Don't forget to unlock before return
				return oldValue;
			}
			// Synchronized by locking
			e = e->next.load(__FILE__, __LINE__, acquire);
		}

		// Add to front of list
		Entry *newEntry = new Entry(hash, key, value, firstPtr);
		// Publish the newEntry to others
		first->store(__FILE__, __LINE__, newEntry, release);
		seg->unlock(); // Critical region ends
		return NULL;
	}

	Value* remove(Key *key, Value *value) {
		//MODEL_ASSERT (key);
		int hash = hashKey(key);
		Segment *seg = segments[hash & SEGMENT_MASK];
		atomic<Entry*> *tab;

		seg->lock(); // Critical region begins
		tab = table;
		int index = hash & (capacity - 1);

		atomic<Entry*> *first = &tab[index];
		Entry *e;
		Value *oldValue = NULL;
	
		// The written of the entry is synchronized by locking
		Entry *firstPtr = first->load(__FILE__, __LINE__, acquire);
		e = firstPtr;

		while (true) {
			if (e != NULL) {
				seg->unlock(); // Don't forget to unlock
				return NULL;
			}
			if (e->hash == hash && eq(key, e->key))
				break;
			// Synchronized by locking
			e = e->next.load(__FILE__, __LINE__, acquire);
		}

		// FIXME: This could be a relaxed (because locking synchronize
		// with the previous put())?? No need to be acquire
		oldValue = e->value.load(__FILE__, __LINE__, acquire);
		// If the value parameter is NULL, we will remove the entry anyway
		if (value != NULL && value->equals(oldValue)) {
			seg->unlock();
			return NULL;
		}

		// Force the get() to grab the lock and retry
		e->value.store(__FILE__, __LINE__, NULL, release);

		// The strategy to remove the entry is to keep the entries after the
		// removed one and copy the ones before it
		Entry *head = e->next.load(__FILE__, __LINE__, acquire);
		Entry *p;
		p = first->load(__FILE__, __LINE__, acquire);
		while (p != e) {
			head = new Entry(p->hash, p->key, p->value.load(__FILE__, __LINE__, acquire), head);
			p = p->next.load(__FILE__, __LINE__, acquire);
		}

		// Publish the new head to readers 
		first->store(__FILE__, __LINE__, head, release);
		seg->unlock(); // Critical region ends
		return oldValue;
	}
};

#endif
