/**
 * @file runtime.h
 * File that contains headers of the runtime functions specified in the specification's document.
 *
 * @author Maxence WO
 */

#ifndef RUNTIME_H
#define RUNTIME_H
#define FUEL_INIT 358

#include <pi_thread.h>

extern int PIT_GC2(PIT_SchedPool schedpool);

extern PIT_SchedPool PIT_create_sched_pool();
extern PIT_Channel  *PIT_create_channel();
extern PIT_PiThread *PIT_create_pithread();
extern PIT_Clock    *PIT_create_clock();
extern PIT_Commit  *PIT_create_commitment();
extern PIT_CommitList *PIT_create_commit_list();
extern PIT_CommitListElement *PIT_create_commit_list_element();

//extern void PIT_sched_pool_slave(PIT_SchedPool schedpool, PIT_Error* error);
extern void PIT_sched_pool_slave(PIT_Args *args);
extern void PIT_sched_pool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);

extern void PIT_main(int nb_core_threads, PIT_PiThreadProc entrypoint);

extern void PIT_register_ouput_commitment(PIT_PiThread *p, PIT_Channel *ch, PIT_EvalFunction *f, int cont_pc);
extern void PIT_register_input_commitment(PIT_PiThread *p, PIT_Channel *ch, int x, int cont_pc);
extern bool PIT_is_valid_commit(PIT_Commit *commit);
extern int  PIT_can_awake(PIT_PiThread p, PIT_Commit c);
extern void PIT_awake(PIT_SchedPool sched, PIT_PiThread p);
extern void PIT_channel_incr_ref_count(PIT_Channel ch, PIT_Error *error);
extern void PIT_channel_dec_ref_count(PIT_Channel ch, PIT_Error *error);

/*################### LIST UTILS ####################*/

extern void       PIT_commit_list_add(PIT_CommitList *clist, PIT_Commit *c);
extern PIT_Commit PIT_commit_list_fetch(PIT_CommitList *clist);

/*####################################################*/

/*################# QUEUE UTILS ######################*/

extern void         PIT_ready_queue_push(PIT_ReadyQueue *rq, PIT_PiThread p);
extern void         PIT_ready_queue_add(PIT_ReadyQueue *rq, PIT_PiThread p);
extern PIT_PiThread PIT_ready_queue_pop(PIT_ReadyQueue *rq);
extern int          PIT_ready_queue_size(PIT_ReadyQueue *rq);
extern void         PIT_wait_queue_push(PIT_WaitQueue *wq, PIT_PiThread p);
extern PIT_PiThread PIT_wait_queue_fetch(PIT_WaitQueue *wq, PIT_PiThread p);
extern void         PIT_wait_queue_push_old(PIT_WaitQueue *wq, PIT_PiThread p);
extern PIT_PiThread PIT_wait_queue_pop_old(PIT_WaitQueue *wq);
extern int          PIT_wait_queue_size(PIT_WaitQueue *wq);
extern int          PIT_wait_queue_max_active(PIT_WaitQueue *wq);
extern int          PIT_wait_queue_max_active_reset(PIT_WaitQueue *wq);

extern PIT_KnownsSet PIT_knows_set_knows(PIT_Knowns* ks);
extern PIT_KnownsSet PIT_knows_set_forget(PIT_Knowns* ks);
extern bool          PIT_knows_register(PIT_KnownsSet ks, PIT_Channel ch);

extern void PIT_acquire_int(PIT_AtomicInt *int_val);
extern void PIT_release_int(PIT_AtomicInt *int_val,PIT_Error *error);
extern void PIT_acquire_bool(PIT_AtomicBoolean *bool_val);
extern void PIT_release_bool(PIT_AtomicBoolean *bool_val,PIT_Error *error);

extern PIT_Commit PIT_fetch_commitment(PIT_Channel *ch);
extern void PIT_cond_wait(PIT_Condition cond, PIT_Mutex lock);


#endif
