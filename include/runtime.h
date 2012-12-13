/**
 * @file runtime.h
 * File that contains headers of the runtime functions specified in the specification's document.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#ifndef RUNTIME_H
#define RUNTIME_H
#define FUEL_INIT 358

#include <pi_thread.h>

extern int PICC_GC2(PICC_SchedPool schedpool);

extern PICC_SchedPool PICC_create_sched_pool();
extern PICC_Channel  *PICC_create_channel();
extern PICC_PiThread *PICC_create_pithread();
extern PICC_Clock    *PICC_create_clock();
extern PICC_Commit  *PICC_create_commitment();
extern PICC_CommitList *PICC_create_commit_list();
extern PICC_CommitListElement *PICC_create_commit_list_element();

extern void PICC_sched_pool_slave(PICC_Args *args);
extern void PICC_sched_pool_master(PICC_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);

extern void PICC_main(int nb_core_threads, PICC_PiThreadProc entrypoint);

extern void PICC_register_ouput_commitment(PICC_PiThread *p, PICC_Channel *ch, PICC_EvalFunction *f, int cont_pc);
extern void PICC_register_input_commitment(PICC_PiThread *p, PICC_Channel *ch, int x, int cont_pc);
extern bool PICC_is_valid_commit(PICC_Commit *commit, PICC_Error *error);
extern int  PICC_can_awake(PICC_PiThread p, PICC_Commit c);
extern void PICC_awake(PICC_SchedPool sched, PICC_PiThread p);
extern void PICC_channel_incr_ref_count(PICC_Channel ch, PICC_Error *error);
extern void PICC_channel_dec_ref_count(PICC_Channel ch, PICC_Error *error);

/*################### LIST UTILS ####################*/

extern void       PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *c);
extern PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist);

/*####################################################*/


extern PICC_KnownsSet PICC_knows_set_knows(PICC_Knowns* ks);
extern PICC_KnownsSet PICC_knows_set_forget(PICC_Knowns* ks);
extern bool          PICC_knows_register(PICC_KnownsSet ks, PICC_Channel ch);

extern void PICC_acquire_int(PICC_AtomicInt *int_val);
extern void PICC_release_int(PICC_AtomicInt *int_val,PICC_Error *error);
extern void PICC_acquire_bool(PICC_AtomicBoolean *bool_val);
extern void PICC_release_bool(PICC_AtomicBoolean *bool_val,PICC_Error *error);

extern PICC_Commit PICC_fetch_commitment(PICC_Channel *ch);
extern void PICC_cond_wait(PICC_Condition cond, PICC_Mutex lock);


#endif
