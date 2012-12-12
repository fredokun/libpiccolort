/**
 * @file runtime.c
 * File that contains definitions of the prototypes specified in runtime.h
 * 
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Joel HING
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#include <definitions.h>
#include <runtime.h>
#include <pi_thread.h>



/**
 * Create and init an atomic boolean
 *
 * @return created atomic boolean
 */
PIT_AtomicBoolean PIT_create_atomic_boolean()
{
	PIT_AtomicBoolean ab;
	pthread_mutex_init(&ab.lock, NULL);
	ab.value = false;
	return ab;
}

/**
 * Acquire a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_acquire_int(PIT_AtomicInt *int_val)
{
	pthread_mutex_lock(int_val->lock);
}	

/**
 * Release a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_release_int(PIT_AtomicInt *int_val, PIT_Error *error)
{
	if (pthread_mutex_trylock(int_val->lock) == 0)
	{
		NEW_ERROR(error,ERR_KERNEL_ERROR);
	}
	else
	{
		pthread_mutex_unlock(int_val->lock);
	}
	
}

/**
 * Acquire a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_acquire_bool(PIT_AtomicBoolean *bool_val)
{
	pthread_mutex_lock(bool_val->lock);
}	

/**
 * Release a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_release_bool(PIT_AtomicBoolean *bool_val, PIT_Error *error)
{
	if (pthread_mutex_trylock(bool_val->lock) == 0)
	{
		NEW_ERROR(error, ERR_KERNEL_ERROR);
	}
	else
	{
		pthread_mutex_unlock(bool_val->lock);
	}
}

/**
 * Second generation garbage collector.
 *
 * @param schedpool The schedpool to clean.
 * @return ? ????????????????
 */
int PIT_GC2(PIT_SchedPool schedpool)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Create a channel which contains 10 commitments
 *
 * @return created channel
 */

PIT_Channel *PIT_create_channel()
{
	PIT_Channel *channel = (PIT_Channel *)malloc( sizeof(PIT_Channel));
	channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );

	return channel;

}

/**
 * Create a channel which contains commit_size commitments
 *
 * @return created channel
 */

PIT_Channel *PIT_create_channel_cn( int commit_size )
{
	PIT_Channel * channel = (PIT_Channel *)malloc( sizeof(PIT_Channel));
	channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->lock = PIT_create_atomic_boolean();
	return channel;

}

/**
 * Function that creates a PIT_PiThread.
 * @return PIT_PiThread a fresh new created PiThread
 */
PIT_PiThread *PIT_create_pithread()
{
	PIT_PiThread *new_thread = (PIT_PiThread*)malloc(sizeof(PIT_PiThread));
	new_thread->knowns = (PIT_KnownsSet)malloc(sizeof(PIT_KnownsSet));
	new_thread->fuel = FUEL_INIT;
	return new_thread;
}

/**
 * Function that creates a clock
 * @return PIT_Clock a fresh new created clock
 */
PIT_Clock *PIT_create_clock()
{
	PIT_Clock * new_clock = (PIT_Clock*)malloc(sizeof(PIT_Clock));
	return new_clock;
}

/**
 * Function that handle the behavior of a sched_pool
 * @param sched_pool the sched_pool to manage
 */
void PIT_sched_pool_slave(PIT_Args *args)
{
	PIT_SchedPool *sched_pool = args->sched_pool;
	PIT_Error *error = &(args->error);
	
	PIT_PiThread current;
	
	while(sched_pool->running)
	{
		while(PIT_ready_queue_size(sched_pool->ready))
		{
			current = PIT_ready_queue_pop(sched_pool->ready);
			do
			{
				current.proc(*sched_pool, current);
			} while(current.status == STATUS_CALL);
			
			if(current.status == STATUS_BLOCKED) // && safe_choice
				NEW_ERROR(error,ERR_DEADLOCK);
		}
		
		PIT_acquire_mutex(sched_pool->lock);
		sched_pool->nb_waiting_slaves++;
		PIT_cond_wait(sched_pool->cond, sched_pool->lock);
		sched_pool->nb_waiting_slaves--;
		PIT_release_mutex(sched_pool->lock,error);

	
	}
}

void PIT_cond_wait(PIT_Condition cond, PIT_Mutex lock)
{
	pthread_cond_wait(cond, lock);
}

/**
 * ????????????????????
 * @param sched_pool ????????????????
 * @param std_gc_fuel ?????????????
 * @param quick_gc_fuel ????????????
 * @param active_factor ?????????????
 */
void PIT_sched_pool_master(PIT_SchedPool sched_pool, int std_gc_fuel, int quick_gc_fuel, int active_factor)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Creates and returns a commitment
 *
 * @return an input commitment
 */
PIT_Commit *PIT_create_commitment()
{
	PIT_Commit *new_commit = (PIT_Commit*)malloc(sizeof(PIT_Commit));
	new_commit->thread = malloc(sizeof(PIT_PiThread));
	new_commit->clock = malloc(sizeof(PIT_Clock));
	new_commit->channel = malloc(sizeof(PIT_Channel));
	
	return new_commit;
}

/**
 * Function that register an output PIT_Commit according to pithread and channel
 *
 * @param the PIT_PiThread used to create the output PIT_Commit
 * @param the PIT_Channel used to create the ourpur PIT_Commit
 * @param refvar the index of the var used to create the output PIT_Commit
 * @param cont_pc
 */
void PIT_register_out_commitment(PIT_PiThread *pi_thread, PIT_Channel *channel, PIT_EvalFunction *function, int cont_pc)
{
	PIT_OutCommit *out = (PIT_OutCommit *)malloc(sizeof(PIT_OutCommit));
	out->eval_func = function;

	PIT_Commit *out_commit = PIT_create_commitment();
	out_commit->content.out = out;
	out_commit->thread = pi_thread; 
	out_commit->cont_pc = cont_pc;
	out_commit->type = OUT_COMMIT;
	out_commit->clock = pi_thread->clock;
	out_commit->clockval = pi_thread->clock->val;
	out_commit->channel = channel;
	PIT_commit_list_add(pi_thread->commits ,out_commit);	
}

/**
 * Function that register an input PIT_Commit according to pithread and channel
 * @param the PIT_PiThread used to create the input PIT_Commit
 * @param the PIT_Channel used to create the input PIT_Commit
 * @param refvar the index of the var used to create the input PIT_Commit
 * @param cont_pc 
 */
void PIT_register_in_commitment(PIT_PiThread *pi_thread, PIT_Channel *channel, int refvar, int cont_pc)
{ 
	PIT_InCommit *in = (PIT_InCommit *)malloc(sizeof(PIT_InCommit));
	in->refvar = refvar;

	PIT_Commit *in_commit = PIT_create_commitment();
	in_commit->content.in = in;
	in_commit->thread = pi_thread;
	in_commit->cont_pc = cont_pc;
	in_commit->type = IN_COMMIT;
	in_commit->clock = pi_thread->clock;
	in_commit->clockval = pi_thread->clock->val;
	in_commit->channel = channel;
	PIT_commit_list_add(pi_thread->commits ,in_commit);	
}
/**
 * Function that verify if the PIT_Commit is valid
 * @param the PIT_Commit whose validity to check
 * @return true if the PIT_Commit is valid, false otherwise
 */
bool PIT_is_valid_commit(PIT_Commit *commit)
{
	PIT_acquire_int(commit->thread->clock->val);
	if(commit->clock == commit->thread->clock)
		if(commit->clockval->value == commit->thread->clock->val->value)
		{
			PIT_release_int(commit->thread->clock->val);
			return true;
		}
	PIT_release_int(commit->thread->clock->value);
	return false;
}

/**
 * Function that fetch a PIT_Commit from a PIT_Channel
 * @param the PIT_Channel in which to fetch
 * @return the PIT_Commit fetched
 */
PIT_Commit PIT_fetch_commitment(PIT_Channel channel)
{
	PIT_Commit current_commit;
	
	return current_commit;
}

/**
 * Function that register an output commitment according to pithread and channel
 * @param p PiThread
 * @param c PIT_Commit
 * @return ? ?????????????
 */
int PIT_can_awake(PIT_PiThread p, PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * ?????????????
 * @param sched ???
 * @param p ????
 * @return ? ?????????????
 */
void PIT_awake(PIT_SchedPool sched, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Increments by 1 the global reference count of a channel
 *
 * @param channel to update
 */
void PIT_channel_incr_ref_count(PIT_Channel channel , PIT_Error *error) 
{
	PIT_acquire_bool(channel.lock);
	channel.global_rc += 1;
	PIT_release_bool(channel.lock,error);
}

/**
 * decrements by 1 the global reference count of a channel
 *
 * @param channel to update
 */
void PIT_channel_dec_ref_count( PIT_Channel channel , PIT_Error *error) 
{
	PIT_acquire_bool( channel.lock);
	channel.global_rc -= 1;
	PIT_release_bool( channel.lock,error);
	if(channel.global_rc == 0 )
	{
		PIT_reclaim_channel(channel);
	}
}

/**
 * Function that creates the PIT_CommitList
 * @return the created PIT_CommitList
 */
PIT_CommitList *PIT_create_commit_list()
{
	PIT_CommitList *new_commit_list = (PIT_CommitList)malloc(sizeof(PIT_CommitList));
	new_commit_list->head = NULL;
	new_commit_list->tail = NULL;
	new_commit_list->size = 0;
	return new_commit_list;
}

/**
 * Function that creates the PIT_CommitListElement, an element of the PIT_CommitList
 * @return the created element PIT_CommitListElement, an element of the PIT_CommitList
 */
PIT_CommitListElement *PIT_create_commit_list_element(PIT_Commit *commit)
{
	PIT_CommitListElement *new_commit_list_element = (PIT_CommitListElement)malloc(sizeof(PIT_CommitListElement));
	new_commit_list_element->commit = commit;
	new_commit_list_element->next = NULL;
	return new_commit_list_element;
}

/**
 * Function that add the selected element at the end of the CommitList
 * @param the PIT_CommitList in which to add a PIT_Commit
 * @param the PIT_Commit to add
 */
void PIT_commit_list_add(PIT_CommitList *commit_list, PIT_Commit *commit)
{
	PIT_CommitListElement *new_commit_list_element = PIT_create_commit_list_element(commit);
	commit_list->tail->next = new_commit_list_element;
	commit_list->tail = new_commit_list_element;
	commit_list->size++:	
}

/**
 * Function that removes the first element from the commitlist and returns it
 * @param the PIT_CommitList in which to fetch
 * @return the PIT_Commit to retrieve
 */
PIT_Commit PIT_commit_list_fetch(PIT_CommitList *commit_list)
{
	PIT_CommitListElement *commit_list_element = commit_list->head;
	commit_list->head = commit_list_element.next;
	commit_list->size--;
	commit_list_element->next = NULL;
	return commit_list_element->commit;
}

/**
 * Function that push a PiThread on the top of a readyqueue
 * @param rq PIT_ReaduQueue
 * @param p PIT_PiThread
 */
void PIT_ready_queue_push(PIT_ReadyQueue *rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that add a pithread a the end of a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @param p PIT_PiThread
 */
void PIT_ready_queue_add(PIT_ReadyQueue *rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that pop a pithread from the top of a readyqueue
 * @param rq PIT_ReadyQueue
 * @return PIT_PiThread
 */
PIT_PiThread PIT_ready_queue_pop(PIT_ReadyQueue *rq)
{
	PIT_PiThread p;
	printf("Not implemented yet.\n");
	return p;
}

/**
 * Function that returns the number of elements in a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @return number of elements in the PIT_ReadyQueue
 */
int PIT_ready_queue_size(PIT_ReadyQueue *rq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that pushes a pithread at the top of a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @param p PIT_PiThread
 */
void PIT_wait_queue_push(PIT_WaitQueue *wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that returns the number of elements in a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @return number of elements in the PIT_ReadyQueue
 */
PIT_PiThread PIT_wait_queue_fetch(PIT_WaitQueue *wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return p;
}

/**
 * Function that pushes a PIT_PiThread at the top of the old PiThreads in a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @param p PIT_PiThread
 */
void PIT_wait_queue_push_old(PIT_WaitQueue *wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that pop the older PIT_PiThread from a waitqueue
 * @param wq PIT_WaitQueue
 * @return PIT_PiThread
 */
PIT_PiThread PIT_wait_queue_pop_old(PIT_WaitQueue *wq)
{
	PIT_PiThread p;
	printf("Not implemented yet.\n");
	return p;
}

/**
 * Function that returns the number of elements in a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @return number of elements in PIT_WaitQueue
 */
int PIT_wait_queue_size(PIT_WaitQueue *wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that pop the older PIT_PiThread from a waitqueue
 * @param wq PIT_WaitQueue
 * @return PIT_PiThread
 */
int PIT_wait_queue_max_active(PIT_WaitQueue *wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that reset the counter of active elemnts ina a waitqueue (ie: active elements are now old elements)
 * @param wq PIT_WaitQueue
 * @return Number of elements ????????
 */
int PIT_wait_queue_max_active_reset(PIT_WaitQueue *wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that returns a subset of all KNOWN-STATE in a knowsset
 * @param ks PIT_Knowsset
 * @return PIT_KnowsSet
 */
PIT_KnownsSet PIT_knows_set_knows(PIT_KnownsSet ks)
{
	printf("Not implemented yet.\n");
	return ks;
}

/**
 * Function that returns a subset of all FORGET-STATE in a Knowsset
 * @param ks PIT_Knowsset
 * @return PIT_KnowsSet
 */
PIT_KnownsSet PIT_knows_set_forget(PIT_KnownsSet ks)
{
	printf("Not implemented yet.\n");
	return ks;
}

/**
 * Function that looks for a channel in a PIT_KnownsSet
 * - if the channel is in the PIT_KnownsSet in KNOWN-STATE, it returns false
 * - if the channel is in the PIT_KnownsSet in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PIT_KnownsSet (KNOWS-STATE) then returns true
 * @param ks PIT_Knowsset
 * @param ch PIT_Channel
 * @return b bool
 */
bool PIT_knows_register(PIT_KnownsSet ks, PIT_Channel ch)
{
	printf("Not implemented yet.\n");
	return false;
}



