#include <unrelacy.h>
#include <atomic>

#include "eventcount.h"

template<typename T>
class spsc_queue
{
public:
	spsc_queue()
	{
		node* n = new node ();
		head = n;
		tail = n;
	}

	~spsc_queue()
	{
		//RL_ASSERT(head == tail);
		delete ((node*)head($));
	}

	void enqueue(T data)
	{
		node* n = new node (data);
		head($)->next.store(__FILE__, __LINE__, n, std::memory_order_release);
		head = n;
		ec.signal_relaxed();
	}

	T dequeue()
	{
    		int OK = 0;
    		T data = try_dequeue();
    		for (int i=0; i<LOOP; i++) { //unroll to do experiment
          if (data !=0) {
        		OK = 1;
        		break;
          } else {
        		int cmp = ec.get();
        		data = try_dequeue();
        		if (data) {
                OK = 1;
          			break;
        		}
        		ec.wait(cmp);
        		data = try_dequeue();
        		if (data) {
          			OK = 1;
          			break;
        		}
          }
    		}
    		if (OK == 1)
      			return data;
    		else
      			return 0;
	}

private:
	struct node
	{
		std::atomic<node*> next;
		rl::var<T> data;

		node(T data = T())
			: data(data)
		{
			next.store(__FILE__, __LINE__, 0);
		}
	};

	rl::var<node*> head;
	rl::var<node*> tail;

	eventcount ec;

	T try_dequeue()
	{
		node* t = tail($);
		node* n = t->next.load(__FILE__, __LINE__, std::memory_order_acquire);
		if (0 == n)
			return 0;
		T data = n->data($);
		delete (t);
		tail = n;
		return data;
	}
};
