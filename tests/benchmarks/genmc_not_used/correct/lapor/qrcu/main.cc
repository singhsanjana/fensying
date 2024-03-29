#include "librace.h" 
#include "model-assert.h"
#include "fake.h"
#include "qrcu.cc"

atomic_int x;
atomic_int y;

atomic_int r_x;
atomic_int r_y;

struct qrcu_struct qs;

void *qrcu_reader(void *unused)
{
	int idx = qrcu_read_lock(&qs);
	r_x = x;
	r_x = y;
	qrcu_read_unlock(&qs, idx);
	;
}

void *qrcu_updater(void *unused)
{

	x = 1;
	synchronize_qrcu(&qs);
	y = 1;
	;
}
