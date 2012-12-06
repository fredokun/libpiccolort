/**
 * @file runtime.h
 * File that contains headers of the runtime functions specified in the specification's document.
 *
 * @author Maxence WO
 */

#ifndef RUNTIME_H
#define RUNTIME_H

#include <pi_thread.h>

extern int PIT_GC2(PIT_SchedPool schedpool);

extern PIT_Channel  PIT_generate_channel();
extern PIT_PiThread PIT_generate_pithread();
extern PIT_Clock    PIT_generate_clock();

extern void PIT_sched_pool_slave(PIT_SchedPool schedpool);
extern void PIT_sched_pool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);


extern void PIT_register_ouput_commitment(PIT_PiThread p, PIT_Channel ch, PIT_EvalFunction* v, int cont_pc);
extern void PIT_register_input_commitment(PIT_PiThread p, PIT_Channel ch, int x, int cont_pc);
extern bool PIT_is_valid_commit(PIT_Commit c);
extern int  PIT_can_awake(PIT_PiThread p, PIT_Commit c);
extern void PIT_awake(PIT_SchedPool sched, PIT_PiThread p);
extern void PIT_channel_incr_ref_count(PIT_Channel ch);
extern void PIT_channel_dec_ref_count(PIT_Channel ch);

/*################### LIST UTILS ####################*/

extern void       PIT_commit_list_add(PIT_Commit* clist, PIT_Commit c);
extern PIT_Commit PIT_commit_list_fetch(PIT_Commit* clist);

/*####################################################*/

/*################# QUEUE UTILS ######################*/

extern void         PIT_ready_queue_push(PIT_ready_queue rq, PIT_PiThread p);
extern void         PIT_ready_queue_add(PIT_ready_queue rq, PIT_PiThread p);
extern PIT_PiThread PIT_ready_queue_pop(PIT_ready_queue rq);
extern int          PIT_ready_queue_size(PIT_ready_queue rq);
extern void         PIT_wait_queue_push(PIT_wait_queue wq, PIT_PiThread p);
extern PIT_PiThread PIT_wait_queue_fetch(PIT_wait_queue wq, PIT_PiThread p);
extern void         PIT_wait_queue_push_old(PIT_wait_queue wq, PIT_PiThread p);
extern PIT_PiThread PIT_wait_queue_pop_old(PIT_wait_queue wq);
extern int          PIT_wait_queue_size(PIT_wait_queue wq);
extern int          PIT_wait_queue_max_active(PIT_wait_queue wq);
extern int          PIT_wait_queue_max_active_reset(PIT_wait_queue wq);

extern PIT_KnownsSet PIT_knows_set_knows(PIT_KnownsSet ks);
extern PIT_KnownsSet PIT_knows_set_forget(PIT_KnownsSet ks);
extern bool          PIT_knows_register(PIT_KnownsSet ks, PIT_Channel ch);

extern void PIT_acquire(PIT_AtomicBoolean b);
extern void PIT_release(PIT_AtomicBoolean b);

extern PIT_Commit PIT_fetch_commitment(PIT_Channel ch);

#endif
