/**
 * @file runtime.h
 * File that contains headers of the runtime functions specified in the specification's document.
 * Authors: Maxence WO
 */

#ifndef DEF_RUNTIME
#define DEF_RUNTIME

#include <pi_thread.h>

int		PIT_GC2(PIT_SchedPool schedpool);

PIT_Channel	PIT_generate_channel();
PIT_PiThread	PIT_generate_pithread();
PIT_Clock	PIT_generate_clock();

void		PIT_SchedPool_slave(PIT_SchedPool schedpool);
void		PIT_SchedPool_master(PIT_SchedPool schedpool, int std_gc_fuel, int quick_gc_fuel, int active_factor);


//definir le type de v
void		PIT_register_ouput_commitment(PIT_PiThread p, PIT_Channel ch, void* v, int cont_pc);
void		PIT_register_input_commitment(PIT_PiThread p, PIT_Channel ch, int x, int cont_pc);
bool		PIT_is_valid_commit(PIT_Commit c);
int		PIT_can_awake(PIT_PiThread p, PIT_Commit c);
void		PIT_awake(PIT_SchedPool sched, PIT_PiThread p);
void		PIT_Channel_incr_ref_count(PIT_Channel ch);
void		PIT_Channel_dec_ref_count(PIT_Channel ch);

/*################### LIST UTILS ####################*/

void 		PIT_Commit_list_add(PIT_Commit* clist, PIT_Commit c);
PIT_Commit 	PIT_Commit_list_fetch(PIT_Commit* clist);

/*####################################################*/

/*################# QUEUE UTILS ######################*/

void 		PIT_ReadyQueue_push(PIT_ReadyQueue rq, PIT_PiThread p);
void 		PIT_ReadyQueue_add(PIT_ReadyQueue rq, PIT_PiThread p);
PIT_PiThread	PIT_ReadyQueue_pop(PIT_ReadyQueue rq);
int 		PIT_ReadyQueue_size(PIT_ReadyQueue rq);
void 		PIT_WaitQueue_push(PIT_WaitQueue wq, PIT_PiThread p);
PIT_PiThread 	PIT_WaitQueue_fetch(PIT_WaitQueue wq, PIT_PiThread p);
void 		PIT_WaitQueue_push_old(PIT_WaitQueue wq, PIT_PiThread p);
PIT_PiThread 	PIT_WaitQueue_pop_old(PIT_WaitQueue wq);
int 		PIT_WaitQueue_size(PIT_WaitQueue wq);
int 		PIT_WaitQueue_max_active(PIT_WaitQueue wq);
int 		PIT_WaitQueue_max_active_reset(PIT_WaitQueue wq);

Knowsset 	PIT_knows_set_knows(Knowsset ks);
KnowsSet 	PIT_knows_set_forget(Knowsset ks);
bool 		PIT_knows_register(Knowsset ks, Channel ch);

void		PIT_acquire(PIT_AtomicBoolean b);
void		PIT_release(PIT_AtomicBoolean b);

PIT_Commit	PIT_fetch_commitment(PIT_Channel ch);

#endif
