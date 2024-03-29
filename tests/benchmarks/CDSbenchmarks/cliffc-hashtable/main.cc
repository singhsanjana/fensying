#include <threads.h>
#include "cliffc_hashtable.h"

using namespace std;

template<typename TypeK, typename TypeV>
slot* const cliffc_hashtable<TypeK, TypeV>::MATCH_ANY = new slot(false, NULL);

template<typename TypeK, typename TypeV>
slot* const cliffc_hashtable<TypeK, TypeV>::NO_MATCH_OLD = new slot(false, NULL);

template<typename TypeK, typename TypeV>
slot* const cliffc_hashtable<TypeK, TypeV>::TOMBPRIME = new slot(true, NULL);

template<typename TypeK, typename TypeV>
slot* const cliffc_hashtable<TypeK, TypeV>::TOMBSTONE = new slot(false, NULL);


class IntWrapper {
	private:
		public:
	    int _val;

		IntWrapper(int val) : _val(val) {}

		IntWrapper() : _val(0) {}

		IntWrapper(IntWrapper& copy) : _val(copy._val) {}

		int get() {
			return _val;
		}

		int hashCode() {
			return _val;
		}
		
		bool operator==(const IntWrapper& rhs) {
			return false;
		}

		bool equals(const void *another) {
			if (another == NULL)
				return false;
			IntWrapper *ptr =
				(IntWrapper*) another;
			return ptr->_val == _val;
		}
};

cliffc_hashtable<IntWrapper, IntWrapper> *table;
IntWrapper *val1, *val2;
IntWrapper *k0, *k1, *k2, *k3, *k4, *k5;
IntWrapper *v0, *v1, *v2, *v3, *v4, *v5;

void threadA(void *arg) {
	// IntWrapper *Res;
	// int res;
	// Res = table->put(k3, v3);
	// res = Res == NULL ? 0 : Res->_val;
	// printf("Put1: key_%d, val_%d, res_%d\n", k3->_val, v3->_val, res);

	// Res = table->get(k2);
	// res = Res == NULL ? 0 : Res->_val;
	// printf("Get2: key_%d, res_%d\n", k2->_val, res);

	IntWrapper *Res1, *Res2;
	int res1, res2;
	Res1 = table->put(k2, v2);
	res1 = Res1 == NULL ? 0 : Res1->_val;
	Res2 = table->put(k3, v3);
	res2 = Res2 == NULL ? 0 : Res2->_val;
}

void threadB(void *arg) {
	// IntWrapper *Res;
	// int res;
	// Res = table->put(k2, v2);
	// res = Res == NULL ? 0 : Res->_val;
	// printf("Put3: key_%d, val_%d, res_%d\n", k2->_val, v2->_val, res);

	// Res = table->get(k3);
	// res = Res == NULL ? 0 : Res->_val;
	// printf("Get4: key_%d, res_%d\n", k3->_val, res);

	IntWrapper *Res1, Res2;
	int res1, res2;
	Res1 = table->get(k3);
	res1 = Res1 == NULL ? 0 : Res1->_val;
	Res2 = table->get(k2);
	res2 = Res2 == NULL ? 0 : Res2->_val;

	MODEL_ASSERT(res1 == 0 || res2 != 0);
}

int user_main(int argc, char *argv[]) {
	thrd_t t1, t2;
	table = new cliffc_hashtable<IntWrapper, IntWrapper>(32);
    k1 = new IntWrapper(3);
	k2 = new IntWrapper(5);
	k3 = new IntWrapper(11);
	k4 = new IntWrapper(7);
	k5 = new IntWrapper(13);

	v0 = new IntWrapper(2048);
	v1 = new IntWrapper(1024);
	v2 = new IntWrapper(47);
	v3 = new IntWrapper(73);
	v4 = new IntWrapper(81);
	v5 = new IntWrapper(99);

	thrd_create(&t1, threadA, NULL);
	thrd_create(&t2, threadB, NULL);
	thrd_join(t1);
	thrd_join(t2);
	
	return 0;
}


