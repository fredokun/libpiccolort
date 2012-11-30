/**
 * @file runtime.c
 * File that contains definitions of the prototypes specified in runtime.h
 * Authors: Maxence WO , Dany SIRIPHOL
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
	channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * 10 );
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
	channel->global_rc = 1;
	channel->incommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->outcommits = (PIT_Commit *) malloc( sizeof( PIT_Commit ) * commit_size );
	channel->lock = PIT_create_atomic_boolean();
	return *channel;
}

PIT_PiThread PIT_generate_pithread()
{
  PIT_PiThread new_thread;
	printf("Not implemented yet.\n");
	return new_thread;
}

PIT_Clock PIT_generate_clock()
{
  PIT_Clock new_clock;
	printf("Not implemented yet.\n");
	return new_clock;
}

void PIT_SchedPool_slave(PIT_SchedPool schedpool)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_SchedPool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_register_ouput_commitment(PIT_PiThread p, PIT_Channel ch, void* v, int cont_pc)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_register_input_commitment(PIT_PiThread p, PIT_Channel ch, int x, int cont_pc)
{
	printf("Not implemented yet.\n");
	return;
}

bool PIT_is_valid_commit(PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return false;
}

int PIT_can_awake(PIT_PiThread p, PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return 0;
}

void PIT_awake(PIT_SchedPool sched, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Increments by 1 the global reference count of a channel
 *
 * @param channel to update
 */

void PIT_channel_incr_ref_count( PIT_Channel channel ) 
{
	PIT_acquire( channel.lock);
	channel.global_rc += 1;
	PIT_release( channel.lock);
}

/**
 * decrements by 1 the global reference count of a channel
 *
 * @param channel to update
 */

void PIT_channel_dec_ref_count( PIT_Channel channel ) 
{
	PIT_acquire( channel.lock);
	channel.global_rc -= 1;
	PIT_release( channel.lock);
	if(channel.global_rc == 0 )
	{
		PIT_reclaim_channel(channel);
	}
}

int PIT_is_valid_commit(PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_Commit_list_add : add the selected element at the end of the CommitList
void PIT_Commit_list_add(PIT_Commit* clist, PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_Commit_list_fetch : removes the first element from the commitlist and returns it
PIT_Commit PIT_Commit_list_fetch(PIT_Commit* clist)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_ReadyQueue_push : push a PiThread on the top of a readyqueue
void PIT_ReadyQueue_push(PIT_ReadyQueue rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_ReadyQueue_add : add a pithread a the end of a PIT_ReadyQueue
void PIT_ReadyQueue_add(PIT_ReadyQueue rq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_ReadyQueue_pop : pop a pithread from the top of a readyqueue
PIT_PiThread PIT_ReadyQueue_pop(PIT_ReadyQueue rq)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_ReadyQueue_size : returns the number of elements in a PIT_ReadyQueue
int PIT_ReadyQueue_size(PIT_ReadyQueue rq)
{
	printf("Not implemented yet.\n");
	return 0;
}

// PIT_WaitQueue_push : pushes a pithread at the top of a PIT_WaitQueue
void PIT_WaitQueue_push(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_WaitQueue_fetch : pop a selected PIT_PiThread from a waitqueue
PIT_PiThread PIT_WaitQueue_fetch(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_WaitQueue_push_old : pushes a PIT_PiThread at the top of the old PiThreads in a PIT_WaitQueue
void PIT_WaitQueue_push_old(PIT_WaitQueue wq, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return;
}

// PIT_WaitQueue_pop_old : pop the older PIT_PiThread from a waitqueue
PIT_PiThread PIT_WaitQueue_pop_old(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_WaitQueue_size : returns the number of elements in a PIT_WaitQueue
int PIT_WaitQueue_size(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

// PIT_WaitQueue_max_active : returns the number of active elements in a PIT_WaitQueue
int PIT_WaitQueue_max_active(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

// PIT_WaitQueue_max_active_reset : reset the counter of active elemnts ina a waitqueue (ie: active elements are now old elements)
int PIT_WaitQueue_max_active_reset(PIT_WaitQueue wq)
{
	printf("Not implemented yet.\n");
	return 0;
}

// PIT_knows_set_knows : returns a subset of all KNOWN-STATE in a knowsset
Knowsset PIT_knows_set_knows(Knowsset ks)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_knows_set_forget : returns a subset of all FORGET-STATE in a Knowsset
KnowsSet PIT_knows_set_forget(Knowsset ks)
{
	printf("Not implemented yet.\n");
	return NULL;
}

// PIT_knows_set_forget_to_unknown : switches an element of a Knowsset from the FORGET-STATE to the UNKNOWN-STATE
// PIT_knows_register : looks for a channel in a Knowsset :
// - if the channel is in the Knowsset in KNOWN-STATE, it returns false
// - if the channel is in the Knowsset in FORGET-STATE, it switches it to KNOWN then  returns false
// - else it add the channel in the Knowsset (KNOWS-STATE) then returns true
int PIT_knows_register(Knowsset ks, Channel ch)
{
	printf("Not implemented yet.\n");
	return 0;
}

/**
 * Acquire a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */

void PIT_acquire( PIT_AtomicBoolean ab )
{
	pthread_mutex_lock (ab.lock);
	

/**
 * Release a mutex on an atomic boolean
 *
 * @param the atomic boolean containing the mutex
 */

void PIT_release( PIT_AtomicBoolean ab )
{
	pthread_mutex_unlock (ab.lock);
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
	PIT_AtomicBoolean ab;
	ab.lock = pthread_mutex_init(&ab.lock, NULL);
	ab.value = false;
	return ab;
}


