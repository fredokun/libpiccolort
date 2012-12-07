/**
 * @file runtime.c
 * File that contains all the necesseray tests to check the behavior of all functions of runtime.c
 * 
 * @author Maxence WO
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <pi_thread.h>
#include <runtime.h>

//int PIT_GC2(PIT_SchedPool schedpool);

/**
 * Test : PIT_create_pithread
 *
 * @return boolean true if it works else false
 */
int check_pithread(PIT_Error error)
{
	PIT_PiThread *p = PIT_create_pithread;
	if(p == NULL)
	{
		return false;
	}
	

	free(p);
	
	return false;
}

/*
PIT_SchedPool PIT_create_sched_pool();
PIT_Channel  *PIT_create_channel();
PIT_PiThread *PIT_create_pithread();
PIT_Clock    PIT_create_clock();
PIT_Commit  *PIT_create_commitment();
PIT_CommitList *PIT_create_commit_list();
PIT_CommitListElement *PIT_create_commit_list_element();

void PIT_sched_pool_slave(PIT_SchedPool schedpool, PIT_Error* error);
void PIT_sched_pool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);

void PIT_main(int nb_core_threads, PIT_PiThreadProc entrypoint);

void PIT_register_ouput_commitment(PIT_PiThread p, PIT_Channel ch, PIT_EvalFunction f, int cont_pc);
void PIT_register_input_commitment(PIT_PiThread p, PIT_Channel ch, int x, int cont_pc);
bool PIT_is_valid_commit(PIT_Commit commit);
int  PIT_can_awake(PIT_PiThread p, PIT_Commit c);
void PIT_awake(PIT_SchedPool sched, PIT_PiThread p);
void PIT_channel_incr_ref_count(PIT_Channel ch);
void PIT_channel_dec_ref_count(PIT_Channel ch);



void       PIT_commit_list_add(PIT_Commit* clist, PIT_Commit c);
PIT_Commit PIT_commit_list_fetch(PIT_Commit* clist);



void         PIT_ready_queue_push(PIT_ReadyQueue *rq, PIT_PiThread p);
void         PIT_ready_queue_add(PIT_ReadyQueue *rq, PIT_PiThread p);
PIT_PiThread PIT_ready_queue_pop(PIT_ReadyQueue *rq);
int          PIT_ready_queue_size(PIT_ReadyQueue *rq);
void         PIT_wait_queue_push(PIT_WaitQueue *wq, PIT_PiThread p);
PIT_PiThread PIT_wait_queue_fetch(PIT_WaitQueue *wq, PIT_PiThread p);
void         PIT_wait_queue_push_old(PIT_WaitQueue *wq, PIT_PiThread p);
PIT_PiThread PIT_wait_queue_pop_old(PIT_WaitQueue *wq);
int          PIT_wait_queue_size(PIT_WaitQueue *wq);
int          PIT_wait_queue_max_active(PIT_WaitQueue *wq);
int          PIT_wait_queue_max_active_reset(PIT_WaitQueue *wq);

PIT_KnownsSet PIT_knows_set_knows(PIT_Knowns* ks);
PIT_KnownsSet PIT_knows_set_forget(PIT_Knowns* ks);
bool          PIT_knows_register(PIT_KnownsSet ks, PIT_Channel ch);

void PIT_acquire_int(PIT_AtomicInt int_val);
void PIT_release_int(PIT_AtomicInt int_val);
void PIT_acquire_bool(PIT_AtomicBoolean bool_val);
void PIT_release_bool(PIT_AtomicBoolean bool_val);

PIT_Commit PIT_fetch_commitment(PIT_Channel ch);

PIT_Clock PIT_create_clock();

*/
