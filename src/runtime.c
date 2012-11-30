/**
 * @file runtime.c
 * File that contains definitions of the prototypes specified in runtime.h
 * Authors: Maxence WO
 */
#include <stdio.h>
#include <stdlib.h>

#include "../include/runtime.h"

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
 * Function that generate a PIT_Channel.
 * @return PIT_Channel 
 */
PIT_Channel PIT_generate_channel()
{
	printf("Not implemented yet.\n");
	return NULL;
}

PIT_PiThread PIT_generate_pithread()
{
	printf("Not implemented yet.\n");
	return NULL;
}

PIT_Clock PIT_generate_clock()
{
	printf("Not implemented yet.\n");
	return NULL;
}

void PIT_scheduler_slave(PIT_SchedPool schedpool)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_scheduler_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor)
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

int PIT_is_valid_commit(PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return 0;
}

int PIT_can_awake(PIT_PiThread p, PIT_Commit c)
{
	printf("Not implemented yet.\n");
	return 0;
}

void PIT_awake(PIT_scheduler sched, PIT_PiThread p)
{
	printf("Not implemented yet.\n");
	return 0;
}

void PIT_Channel_incr_ref_count(PIT_Channel ch)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_Channel_dec_ref_count(PIT_Channel ch)
{
	printf("Not implemented yet.\n");
	return;
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

/* ATOMIC */
void PIT_acquire(PIT_AtomicBoolean b)
{
	printf("Not implemented yet.\n");
	return;
}

void PIT_release(PIT_AtomicBoolean b)
{
	printf("Not implemented yet.\n");
	return;
}

PIT_Commit PIT_fetch_commitment(PIT_Channel ch)
{
	printf("Not implemented yet.\n");
	return NULL;
}
