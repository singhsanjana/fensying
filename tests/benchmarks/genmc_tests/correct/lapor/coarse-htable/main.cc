#include "librace.h" 
#include "model-assert.h"
#include "coarse-htable.cc"

/* Driver code */
#ifndef MAX_THREADS
# define MAX_THREADS 32
#endif
#ifndef MAX_FREELIST
# define MAX_FREELIST 32 /* Each thread can own up to MAX_FREELIST free nodes */
#endif

#ifdef CONFIG_HTABLE_ADDERS
# define DEFAULT_ADDERS (CONFIG_HTABLE_ADDERS)
#else
# define DEFAULT_ADDERS 2
#endif
#ifdef CONFIG_HTABLE_SEEKERS
# define DEFAULT_SEEKERS (CONFIG_HTABLE_SEEKERS)
#else
# define DEFAULT_SEEKERS 0
#endif
#ifdef CONFIG_HTABLE_REMOVERS
# define DEFAULT_REMOVERS (CONFIG_HTABLE_REMOVERS)
#else
# define DEFAULT_REMOVERS 0
#endif

static int adders = DEFAULT_ADDERS, seekers = DEFAULT_SEEKERS, removers = DEFAULT_REMOVERS;
static int num_threads;

DEFINE_HTABLE(myht);

thrd_t threads[MAX_THREADS + 1];
int param[MAX_THREADS + 1];
struct htable_entry free_lists[MAX_THREADS + 1][MAX_FREELIST];
unsigned int free_index[MAX_THREADS + 1];

int __thread tid;

void set_thread_num(int i)
{
	tid = i;
}

int get_thread_num()
{
	return tid;
}

struct htable_entry *new_node(int val)
{
	int t = get_thread_num();

	MODEL_ASSERT(free_index[t] < MAX_FREELIST);
	free_lists[t][free_index[t]].val = val;
	INIT_LIST_HEAD(&free_lists[t][free_index[t]].list);
	return &free_lists[t][free_index[t]++];
}

/* Should be called before threads are spawned (from user_user_user_main()) */
void init()
{
	num_threads = adders + seekers + removers + 1;
	for (int j = 0; j < num_threads; j++)
		param[j] = j;

	htable_init(&myht);
}

#define MAX 2
#define NO_COLLISION_LIMIT 4

void *thread_add(void *tid)
{
	int t = (*(int *) tid);
	set_thread_num(t);

	for (int i = 0; i < MAX; i++)
		add(&myht, i * NO_COLLISION_LIMIT + t);
	;
}

void *thread_seek(void *tid)
{
	int t = (*(int *) tid);
	set_thread_num(t);

	for (int i = 0; i < MAX; i++)
		contains(&myht, i * NO_COLLISION_LIMIT + t);
	;
}

void *thread_del(void *tid)
{
	;
}
