#include <stdatomic.h>
#include <unrelacy.h>

#define LOOP_BOUND 2

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
		unsigned int rdwr = m_rdwr.load(__FILE__, __LINE__, mo_relaxed); // mo_acquire);
		unsigned int rd,wr;
		int loop_iter = 0;
		for(loop_iter = 0; loop_iter < LOOP_BOUND; loop_iter++) {
			rd = (rdwr>>16) & 0xFFFF;
			wr = rdwr & 0xFFFF;

			if ( wr == rd ) // empty
				return NULL;

			if ( m_rdwr.compare_exchange_weak(__FILE__, __LINE__, rdwr,rdwr+(1<<16),mo_relaxed) ) // mo_acq_rel);) )
				break;
		}

		if (loop_iter >= LOOP_BOUND)
			return NULL;

		// (*1)
		rl::backoff bo;
		loop_iter = 0;
		while ( (m_written.load(__FILE__, __LINE__, mo_acquire) & 0xFFFF) != wr && loop_iter++ < LOOP_BOUND ) ;

		if (loop_iter >= LOOP_BOUND) {
			return NULL;
		} 

		t_element * p = & ( m_array[ rd % t_size ] );

		return p;
	}

	void read_consume() {
		m_read.fetch_add(__FILE__, __LINE__, 1,mo_relaxed); // mo_release);
	}

	//-----------------------------------------------------

	t_element * write_prepare() {
		unsigned int rdwr = m_rdwr.load(__FILE__, __LINE__, mo_relaxed); // mo_acquire);
		unsigned int rd,wr;
		int loop_iter = 0;
		for(loop_iter = 0; loop_iter < LOOP_BOUND; loop_iter++) {
			rd = (rdwr>>16) & 0xFFFF;
			wr = rdwr & 0xFFFF;

			if ( wr == ((rd + t_size)&0xFFFF) ) // full
				return NULL;

			if ( m_rdwr.compare_exchange_weak(__FILE__, __LINE__, rdwr,(rd<<16) | ((wr+1)&0xFFFF),mo_relaxed) ) // mo_acq_rel);) )
				break;
		}

		if (loop_iter >= LOOP_BOUND)
			return NULL;

		// (*1)
		rl::backoff bo;
		loop_iter = 0;
		while ( (m_read.load(__FILE__, __LINE__, mo_acquire) & 0xFFFF) != rd  && loop_iter++ < LOOP_BOUND ) ;

		if (loop_iter >= LOOP_BOUND) {
			return NULL;
		} 

		t_element * p = & ( m_array[ wr % t_size ] );

		return p;
	}

	void write_publish()
	{
		m_written.fetch_add(__FILE__, __LINE__, 1,mo_relaxed); // mo_release);
	}

	//-----------------------------------------------------


};
