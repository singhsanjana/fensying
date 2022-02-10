#include <stdatomic.h>
#include <unrelacy.h>

#define LOOP 5

template <typename t_element, size_t t_size>
struct mpmc_boundq_1_alt
{
private:

	// elements should generally be cache-line-size padded :
	t_element               m_array[t_size];

	// rdwr counts the reads & writes that have started
	atomic<unsigned int>    m_rdwr;
	// "read" and "written" count the number completed
	atomic<unsigned int>    m_read;
	atomic<unsigned int>    m_written;

public:

	mpmc_boundq_1_alt()
	{
		m_rdwr = 0;
		m_read = 0;
		m_written = 0;
	}

	//-----------------------------------------------------

	t_element * read_fetch() {
		int OK = 0;
		unsigned int rdwr = m_rdwr.load(__FILE__, __LINE__, memory_order_acquire);
		unsigned int rd,wr;
		for(int i=0; i<LOOP; i++) {
			rd = (rdwr>>16) & 0xFFFF;
			wr = rdwr & 0xFFFF;

			if ( wr == rd ) // empty
				return NULL;
			
			if ( m_rdwr.compare_exchange_weak(__FILE__, __LINE__, rdwr,rdwr+(1<<16),memory_order_acq_rel) ) {
				OK = 1;
				break;
			} 
		}
		
		if (OK==1) {
			OK = 0;
			rl::backoff bo;
			for (int i=0; i<LOOP; i++) {
				if ( (m_written.load(__FILE__, __LINE__, memory_order_acquire) & 0xFFFF) == wr ) {				
					OK = 1;
					break;
				}
			}
		}

		if (OK == 1) {
			t_element * p = & ( m_array[ rd % t_size ] );
			return p;
		} else
		return NULL;
		
	}

	void read_consume() {
		m_read.fetch_add(__FILE__, __LINE__, 1,memory_order_acq_rel);
	}

	t_element * write_prepare() {
		int OK = 0;
		unsigned int rdwr = m_rdwr.load(__FILE__, __LINE__, memory_order_acquire);
		unsigned int rd,wr;
		for(int i=0; i<LOOP; i++) {
			rd = (rdwr>>16) & 0xFFFF;
			wr = rdwr & 0xFFFF;

			if ( wr == ((rd + t_size)&0xFFFF) ) // full
				return NULL;

			if ( m_rdwr.compare_exchange_weak(__FILE__, __LINE__, rdwr,(rd<<16) | ((wr+1)&0xFFFF),memory_order_acq_rel) ) {
				OK = 1;
				break;
			}
		}

		if (OK==1) {
			OK = 0;
			rl::backoff bo;
			for (int i=0; i<LOOP; i++) {
				if ( (m_read.load(__FILE__, __LINE__, memory_order_acquire) & 0xFFFF) == rd ) {
					OK = 1;
					break;
				}
			}
		}

		if (OK == 1) {
			t_element * p = & ( m_array[ wr % t_size ] );
			return p;
		} else {
			return NULL;
		}
		
	}

	void write_publish()
	{
		m_written.fetch_add(__FILE__, __LINE__, 1,memory_order_acq_rel);
	}
};
