// mcs on stack

#include <stdatomic.h>
#include <unrelacy.h>

#define LOOP 5

struct mcs_node {
	std::atomic<mcs_node *> next;
	std::atomic<int> gate;

	mcs_node() {
		next.store(__FILE__, __LINE__, 0, memory_order_release);
		gate.store(__FILE__, __LINE__, 0, memory_order_release);
	}
};

struct mcs_mutex {
public:
	// tail is null when lock is not held
	std::atomic<mcs_node *> m_tail;

	mcs_mutex() {
		m_tail.store(__FILE__, __LINE__,  NULL, memory_order_release);
	}
	~mcs_mutex() {
		ASSERT( m_tail.load(__FILE__, __LINE__, memory_order_acquire) == NULL );
	}

	class guard {
	public:
		mcs_mutex * m_t;
		mcs_node    m_node; // node held on the stack

		guard(mcs_mutex * t) : m_t(t) { t->lock(this); }
		~guard() { m_t->unlock(this); }
	};

	int lock(guard * I) {
		mcs_node * me = &(I->m_node);

		// set up my node :
		// not published yet so relaxed :
		me->next.store(__FILE__, __LINE__, NULL, std::memory_order_release );
		me->gate.store(__FILE__, __LINE__, 1, std::memory_order_release );

		// publish my node as the new tail :
		mcs_node * pred = m_tail.exchange(__FILE__, __LINE__, me, std::mo_acq_rel);
		if ( pred != NULL ) {
			// (*1) race here
			// unlock of pred can see me in the tail before I fill next

			// publish me to previous lock-holder :
			pred->next.store(__FILE__, __LINE__, me, std::mo_release );

			// (*2) pred not touched any more       

			// now this is the spin -
			// wait on predecessor setting my flag -
			rl::linear_backoff bo;

			for (int i=0; i<LOOP; i++) {
				if (!me->gate.load(__FILE__, __LINE__, std::mo_acquire))
					return 1;
			}
		}
		return 0;
	}

	int unlock(guard * I) {
		int OK=0;
		mcs_node * me = &(I->m_node);

		mcs_node * next = me->next.load(__FILE__, __LINE__, std::mo_acquire);
		if ( next == NULL )
		{
			mcs_node * tail_was_me = me;
			if ( m_tail.compare_exchange_strong(__FILE__, __LINE__,  tail_was_me,NULL,std::mo_acq_rel) ) {
				// got null in tail, mutex is unlocked
				return 1;
			}

			// (*1) catch the race :
			rl::linear_backoff bo;
			for(int i=0; i<LOOP; i++) {
				next = me->next.load(__FILE__, __LINE__, std::mo_acquire);
				if ( next != NULL ) {
					OK = 1;
					break;
				}

			}
		}

		// (*2) - store to next must be done,
		//  so no locker can be viewing my node any more        
		if (OK==1) {
			// let next guy in :
			next->gate.store(__FILE__, __LINE__,  0, std::mo_release );
			return 1;
		} else 
			return 0;
	}
};
