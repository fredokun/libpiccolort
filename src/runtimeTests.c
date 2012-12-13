/**
 * @file runtime.c
 * File that contains all the necesseray tests to check the behavior of all functions of runtime.c
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <pi_thread.h>
#include <runtime.h>

//int PICC_GC2(PICC_SchedPool schedpool);

/**
 * Test : PICC_create_pithread
 *
 * @return boolean true if it works else false
 */
bool check_pithread(PIT_Error *error)
{
    PIT_PiThread *p;

    p = PIT_create_pithread();
    if(p == NULL)
    {
        NEW_ERROR(error,ERR_NULLPOINTER_PITHREAD);
        return false;
    }

    free(p);

    return true;
}

bool check_commits(PIT_Error *error)
{
    PIT_Commit *c, *c2, *c3;
    PIT_CommitListElement *clistelem;
    PIT_CommitList *clist;

    c = PICC_create_commitment();
    if(c == NULL)
    {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMIT);
        return false;
    }
    c2 = PICC_create_commitment();
    c3 = PICC_create_commitment();

    //is_valid ?
    if(!PIT_is_valid_commit(c,error))
    {
        NEW_ERROR(error, ERR_INVALID_COMMIT);
    }

    clistelem = PICC_create_commit_list_element();
    if(clistelem == NULL)
    {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMITLISTELEM);
    }

    clist = PICC_create_commit_list();
    if(clist == NULL)
    {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMITLIST);
        return false;
    }

    clist->head = clistelem;
    clist->tail = clistelem;

    c->cont_pc = 1;
    c2->cont_pc = 2;
    c3->cont_pc = 3;

    PIT_commit_list_add(clist, c);
    PIT_commit_list_add(clist, c2);
    PIT_commit_list_add(clist, c3);

    free(c);
    free(c2);
    free(c3);
    free(clistelem);
    free(clist);
}



/*
PICC_SchedPool PICC_create_sched_pool();
PICC_Channel  *PICC_create_channel();
PICC_PiThread *PICC_create_pithread();
PICC_Clock    PICC_create_clock();
PICC_Commit  *PICC_create_commitment();
PICC_CommitList *PICC_create_commit_list();
PICC_CommitListElement *PICC_create_commit_list_element();

void PICC_sched_pool_slave(PICC_SchedPool schedpool, PICC_Error* error);
void PICC_sched_pool_master(PICC_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);

void PICC_main(int nb_core_threads, PICC_PiThreadProc entrypoint);

void PICC_register_ouput_commitment(PICC_PiThread p, PICC_Channel ch, PICC_EvalFunction f, int cont_pc);
void PICC_register_input_commitment(PICC_PiThread p, PICC_Channel ch, int x, int cont_pc);
bool PICC_is_valid_commit(PICC_Commit commit);
int  PICC_can_awake(PICC_PiThread p, PICC_Commit c);
void PICC_awake(PICC_SchedPool sched, PICC_PiThread p);
void PICC_channel_incr_ref_count(PICC_Channel ch);
void PICC_channel_dec_ref_count(PICC_Channel ch);



void       PICC_commit_list_add(PICC_Commit* clist, PICC_Commit c);
PICC_Commit PICC_commit_list_fetch(PICC_Commit* clist);



void         PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread p);
void         PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread p);
PICC_PiThread PICC_ready_queue_pop(PICC_ReadyQueue *rq);
int          PICC_ready_queue_size(PICC_ReadyQueue *rq);
void         PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread p);
PICC_PiThread PICC_wait_queue_fetch(PICC_WaitQueue *wq, PICC_PiThread p);
void         PICC_wait_queue_push_old(PICC_WaitQueue *wq, PICC_PiThread p);
PICC_PiThread PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
int          PICC_wait_queue_size(PICC_WaitQueue *wq);
int          PICC_wait_queue_max_active(PICC_WaitQueue *wq);
int          PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);

PICC_KnownsSet PICC_knows_set_knows(PICC_Knowns* ks);
PICC_KnownsSet PICC_knows_set_forget(PICC_Knowns* ks);
bool          PICC_knows_register(PICC_KnownsSet ks, PICC_Channel ch);

void PICC_acquire_int(PICC_AtomicInt int_val);
void PICC_release_int(PICC_AtomicInt int_val);
void PICC_acquire_bool(PICC_AtomicBoolean bool_val);
void PICC_release_bool(PICC_AtomicBoolean bool_val);

PICC_Commit PICC_fetch_commitment(PICC_Channel ch);

PICC_Clock PICC_create_clock();

*/
