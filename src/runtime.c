/**
 * @file runtime.c
 * File that contains definitions of the prototypes specified in runtime.h
 * 
 * @author Maxence WO
 * @author Dany SIRIPHOL
 */

#include <runtime.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

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
 PIT_Channel PIT_generate_channel()
{
	PIT_Channel *channel = (PIT_Channel *)malloc( sizeof(PIT_Channel));
	/*channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );*/
	return *channel;
}

/**
 * Create a channel which contains commit_size commitments
 *
 * @return created channel
 */
PIT_Channel PIT_generate_channel_cn( int commit_size )
{
	PIT_Channel * channel = (PIT_Channel *)malloc( sizeof(PIT_Channel));
	/*channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->lock = PIT_create_atomic_boolean();*/
	return *channel;
}

/**
 * Function that generate a PIT_PiThread.
 * @return PIT_PiThread a fresh new generated PiThread
 */
PIT_PiThread PIT_generate_pithread()
{
	PIT_PiThread new_thread;
	printf("Not implemented yet.\n");
	return new_thread;
}

/**
 * Function that generate a clock
 * @return PIT_Clock a fresh new generated clock
 */
PIT_Clock PIT_generate_clock()
{
	PIT_Clock new_clock;
	printf("Not implemented yet.\n");
	return new_clock;
}

/**
 * ????????????????????
 * @param schedpool ????????????????
 */
void PIT_SchedPool_slave(PIT_SchedPool schedpool)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * ????????????????????
 * @param schedpool ????????????????
 * @param std_gc_fuel ?????????????
 * @param quick_gc_fuel ????????????
 * @param active_factor ?????????????
 */
void PIT_SchedPool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that register an output commitment according to pithread and channel
 * @param p Pithread
 * @param ch PIT_Channel
 * @param v 
 * @param cont_pc
 */
void PIT_register_ouput_commitment(PIT_PiThread p, PIT_Channel ch, PIT_EvalFunction* v, int cont_pc)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that register an input commitment according to pithread and channel
 * @param p Pithread
 * @param ch PIT_Channel
 * @param v 
 * @param cont_pc 
 */
void PIT_register_input_commitment(PIT_PiThread p, PIT_Channel ch, int x, int cont_pc)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that verify if the commit is valid
 * @param c PIT_Commit
 * @return b bool
 * @return ? ?????????????
 */
bool PIT_is_valid_commit(PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return false;
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
void PIT_channel_incr_ref_count( PIT_Channel channel ) 
{
	PIT_acquire( channel->lock);
	channel->global_rc += 1;
	PIT_release( channel->lock);
}

/**
 * decrements by 1 the global reference count of a channel
 *
 * @param channel to update
 */
void PIT_channel_dec_ref_count( PIT_Channel channel ) 
{
	/*PIT_acquire( channel.lock);
	channel.global_rc -= 1;
	PIT_release( channel.lock);
	if(channel.global_rc == 0 )
	{
		PIT_reclaim_channel(channel);
	}*/
}

/**
 * Function that add the selected element at the end of the CommitList
 * @param clist PIT_Commit
 * @param c PIT_Commit
 */
void PIT_Commit_list_add(PIT_Commit* clist, PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that removes the first element from the commitlist and returns it
 * @param clist PIT_Commit
 * @return PIT_Commit
 */
PIT_Commit PIT_Commit_list_fetch(PIT_Commit* clist)
{
	printf("Not implemented yet.\n");
	return NULL;
}

/**
 * Function that push a PiThread on the top of a readyqueue
 * @param rq PIT_ReaduQueue
 * @param p PIT_PiThread
 */
void PIT_ReadyQueue_push(PIT_ReadyQueue rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that add a pithread a the end of a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @param p PIT_PiThread
 */
void PIT_ReadyQueue_add(PIT_ReadyQueue rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that pop a pithread from the top of a readyqueue
 * @param rq PIT_ReadyQueue
 * @return PIT_PiThread
 */
PIT_PiThread PIT_ReadyQueue_pop(PIT_ReadyQueue rq)
{
	printf("Not implemented yet.\n");
	return NULL;
}

/**
 * Function that returns the number of elements in a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @return number of elements in the PIT_ReadyQueue
 */
int PIT_ReadyQueue_size(PIT_ReadyQueue rq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that pushes a pithread at the top of a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @param p PIT_PiThread
 */
void PIT_WaitQueue_push(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that returns the number of elements in a PIT_ReadyQueue
 * @param rq PIT_ReadyQueue
 * @return number of elements in the PIT_ReadyQueue
 */
PIT_PiThread PIT_WaitQueue_fetch(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return NULL;
}

/**
 * Function that pushes a PIT_PiThread at the top of the old PiThreads in a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @param p PIT_PiThread
 */
void PIT_WaitQueue_push_old(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

/**
 * Function that pop the older PIT_PiThread from a waitqueue
 * @param wq PIT_WaitQueue
 * @return PIT_PiThread
 */
PIT_PiThread PIT_WaitQueue_pop_old(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return NULL;
}

/**
 * Function that returns the number of elements in a PIT_WaitQueue
 * @param wq PIT_WaitQueue
 * @return number of elements in PIT_WaitQueue
 */
int PIT_WaitQueue_size(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that pop the older PIT_PiThread from a waitqueue
 * @param wq PIT_WaitQueue
 * @return PIT_PiThread
 */
int PIT_WaitQueue_max_active(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Function that reset the counter of active elemnts ina a waitqueue (ie: active elements are now old elements)
 * @param wq PIT_WaitQueue
 * @return Number of elements ????????
 */
int PIT_WaitQueue_max_active_reset(PIT_WaitQueue wq)
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
	return NULL;
}

/**
 * Function that returns a subset of all FORGET-STATE in a Knowsset
 * @param ks PIT_Knowsset
 * @return PIT_KnowsSet
 */
PIT_KnownsSet PIT_knows_set_forget(PIT_KnownsSet ks)
{
	printf("Not implemented yet.\n");
	return NULL;
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
	return 0;
}

/**
 * Acquire a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_acquire(PIT_AtomicBoolean ab)
{
	pthread_mutex_lock (ab->lock);
}
	

/**
 * Release a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */
void PIT_release(PIT_AtomicBoolean ab)
{
	pthread_mutex_unlock(ab->lock);
}

PIT_Commit PIT_fetch_commitment(PIT_Channel ch)
{
	printf("Not implemented yet.\n");
	return NULL;
}

/**
 * Create and init an atomic boolean
 *
 * @return created atomic boolean
 */
PIT_AtomicBoolean PIT_create_atomic_boolean()
{
	/*b.lock = pthread_mutex_init(&ab.lock, NULL);
	ab.value = false;
	return ab;
	*/
	return NULL;
}
