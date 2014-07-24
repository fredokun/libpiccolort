/**
 * @file pi_thread.c
 * Pi-Threads
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Sergiu TIGANU
 * @author Aurélien DEHARBE
 */

#include <channel_repr.h>
#include <pi_thread_repr.h>
#include <scheduler_repr.h>
#include <commit_repr.h>
#include <queue_repr.h>
#include <value_repr.h>
#include <atomic_repr.h>
#include <knownset_repr.h>
#include <tools.h>

/**
 * Creates a new PiThread with given environment and knowns set length.
 *
 * @pre env_length >= 0
 * @pre knowns_length >= 0
 *
 * @post thread->env_length == env_length
 * @post thread->enabled == NULL
 * @post thread->enabled_length == 0
 * @post thread->commit == NULL
 * @post thread->proc == NULL
 * @post thread->pc == PICC_DEFAULT_ENTRY_LABEL
 * @post thread->fuel == PICC_FUEL_INIT
 * @post thread->val == NULL
 *
 * @param env_length Size of the environment
 * @param knowns_length Size of the knowns set
 * @return Created PiThread
 */
PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length, int enabled_length)
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(env_length >= 0);
        ASSERT(knowns_length >= 0);
        ASSERT(enabled_length >= 0);
    #endif
	int i;
    PICC_ALLOC_CRASH(thread, PICC_PiThread) {
        ALLOC_ERROR(sub_error);
        /* thread->chans = PICC_create_empty_knownset(); */
        thread->knowns = PICC_create_knownset(knowns_length, &sub_error);
        thread->clock = PICC_create_clock(&sub_error);
        if (HAS_ERROR(sub_error)) {
            CRASH(&sub_error);
        } else {
            PICC_ALLOC_N_CRASH(env, PICC_Value, env_length) {
                
                if (HAS_ERROR(sub_error)) {
                    CRASH(&sub_error);
                } else {
                    thread->commits = PICC_create_commit_list(&sub_error);
                    thread->commit = NULL;
                    if (HAS_ERROR(sub_error)) {
                        CRASH(&sub_error);
                    } else {
                        thread->env = env;
                        thread->env_length = env_length;
                        PICC_ALLOC_N_CRASH(enabled, bool, enabled_length) {
                            thread->enabled = enabled;
                            for(i=0; i<enabled_length; i++){
                                thread->enabled[i] = false;
                            }
                            thread->enabled_length = enabled_length;
                            thread->proc = NULL;
                            thread->pc = PICC_DEFAULT_ENTRY_LABEL;
                            thread->fuel = PICC_FUEL_INIT;
                            PICC_INIT_NO_VALUE(&thread->val);
                            thread->lock = PICC_create_lock(&sub_error);
                            thread->status = PICC_STATUS_RUN;
                            if (HAS_ERROR(sub_error)) {
                                CRASH(&sub_error);
                            }
                        }
                    }
                }
            }
        }
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_PiThread_inv(thread);
    #endif

    #ifdef CONTRACT_POST
        // post
        ASSERT(thread->env_length == env_length);
        ASSERT(thread->enabled_length == enabled_length);
        ASSERT(thread->commit == NULL);
        ASSERT(thread->proc == NULL);
        ASSERT(thread->pc == PICC_DEFAULT_ENTRY_LABEL);
        ASSERT(thread->fuel == PICC_FUEL_INIT);
        ASSERT(thread->val.header == MAKE_HEADER(TAG_NOVALUE,0));
    #endif

    return thread;
}

/**
 * Reclaims the given PiThread.
 *
 * @param pt PiThread to reclaim
 */
void PICC_reclaim_pi_thread(PICC_PiThread *pt)
{
    free(pt->enabled);
    PICC_free_knownset(pt->knowns);
    /* PICC_free_knownset(pt->chans); */
    free(pt->env);
    free(pt->clock);
    free(pt->commits);
    PICC_lock_free(pt->lock);
}


/**
 * Returns whether a PiThread can be awaken with the given commit.
 *
 * @pre PICC_PiThread_inv(pt) must pass
 * @pre PICC_Commit_inv(commit) must pass
 *
 * @post valid commitment implies pt->clock->val <= PICC_CLOCK_MAX_IN
 * @post valid commitment implies pt->commit == commit
 *
 * @param pt PiThread to check
 * @param commit Commitment
 * @return Whether the PiThread can be awaken with given commit
 */
PICC_CommitStatus PICC_can_awake(PICC_PiThread *pt, PICC_Commit *commit)
{
    #ifdef CONTRACT_PRE_INV
        // inv
        PICC_PiThread_inv(pt);
    #endif

    #ifdef CONTRACT_PRE
        // pre
        ASSERT(commit != NULL);
        PICC_Commit_inv(commit);
    #endif

    PICC_CommitStatus status;

    if (!PICC_try_acquire(pt->lock)) {
        status = PICC_CANNOT_ACQUIRE;

    } else if (commit->clock != pt->clock || commit->clockval != PICC_atomic_int_get(commit->clock->val)) {
        PICC_release(pt->lock);
        status = PICC_INVALID_COMMIT;

    } else {        
        pt->commit = commit;
        PICC_release(pt->lock);
        status = PICC_VALID_COMMIT;
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_PiThread_inv(pt);
    #endif

    #ifdef CONTRACT_POST
        //post
        if (status == PICC_VALID_COMMIT) {            
            ASSERT(pt->commit == commit);
        }
    #endif

    return status;
}

/**
 * Awakes a PiThread in the given scheduler.
 *
 * @pre PICC_PiThread_inv(pt) must pass
 * @pre PICC_Commit_inv(commit) must pass
 * @pre sched != NULL
 *
 * @post pt->commit == NULL
 * @post pt->pc == commit->cont_pc
 * @post pt->status == PICC_STATUS_RUN
 *
 * @param sched Scheduler
 * @param pt PiThread to be awaken
 */
void PICC_awake(PICC_SchedPool *sched, PICC_PiThread *pt, PICC_Commit *commit)
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(commit != NULL);
        ASSERT(sched != NULL);
    #endif
    
    #ifdef CONTRACT_PRE_INV
        // inv
        PICC_PiThread_inv(pt);
        PICC_Commit_inv(commit);
    #endif
    
    PICC_acquire(pt->lock);
    
    if (pt->commit != commit) {
        CRASH_NEW_ERROR(ERR_INVALID_COMMIT);
    }
    PICC_PiThread *fetched;
    do {
        fetched = PICC_wait_queue_fetch(sched->wait, pt);
    } while (fetched == NULL);
    pt->commit = NULL;
    pt->pc = commit->cont_pc;
    pt->status = PICC_STATUS_RUN;    

    int clock_val = PICC_atomic_int_get(pt->clock->val);
    if (clock_val == PICC_CLOCK_MAX_INT) {
        PICC_reclaim_clock(pt->clock);
        pt->clock = NULL;
        ALLOC_ERROR(error);
        pt->clock = PICC_create_clock(&error);
        if (HAS_ERROR(error)) {
            CRASH(&error);
        }
    } else {
        PICC_atomic_int_compare_and_swap(pt->clock->val, clock_val, clock_val + 1);
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_PiThread_inv(pt);
        //PICC_WaitQueue_inv(sched->wait); // problems when more then one real thread
        //PICC_ReadyQueue_inv(sched->ready); // problems when more then one real thread
    #endif

    #ifdef CONTRACT_POST
        // post
        ASSERT(pt->commit == NULL);
        ASSERT(pt->pc == commit->cont_pc);
        ASSERT(pt->status == PICC_STATUS_RUN);
        ASSERT(PICC_atomic_int_get(pt->clock->val) <= PICC_CLOCK_MAX_INT);
    #endif
    
    PICC_release(pt->lock);
    PICC_ready_queue_add(sched->ready, pt);
}

/**
 * End a PiThread.
 *
 * @pre pt != NULL
 * @pre PICC_PiThread_inv(pt) must pass
 *
 * @post pt->status == status
 *
 * @param pt the PiThread to end
 * @param status the termination status (Ended or Blocked)
 */
void PICC_process_end(PICC_PiThread *pt, PICC_StatusKind status) {
        
  PICC_KnownValue chan;
  PICC_KnownSet* s=PICC_knownset_known( pt->knowns );
  
  PICC_KNOWNSET_FOREACH(s, 
                        chan){
    PICC_handle_dec_ref_count( &PICC_GET_HANDLE( &chan ) );  
    PICC_free_knownset(s);
  }
      
  s=PICC_knownset_forget( pt->knowns );
  PICC_KNOWNSET_FOREACH(s, 
                        chan){
    PICC_handle_dec_ref_count( &PICC_GET_HANDLE( &chan ) );
    
  }

  PICC_free_knownset(s);
  
  pt->status = status;

}



/**
 * Puts the current posix thread at the end of the system thread queue
 */
void PICC_low_level_yield()
{
    int status = sched_yield();
    if (status) {
        CRASH_NEW_ERROR(ERR_THREAD_YIELD);
    }
}


// Clocks //////////////////////////////////////////////////////////////////////

/**
 * Creates a new clock.
 *
 * @param error Error stack
 * @return Created clock
 */
PICC_Clock *PICC_create_clock(PICC_Error *error)
{
    PICC_ALLOC(clock, PICC_Clock, error) {
        ALLOC_ERROR(sub_error);
        clock->val = PICC_create_atomic_int(0, &sub_error);
        if (HAS_ERROR(sub_error)) {
            ADD_ERROR(error, sub_error, ERR_CLOCK_CREATE);
            free(clock);
            clock = NULL;
        }
    }
    return clock;
}

/**
 * Frees the given clock.
 */
void PICC_reclaim_clock(PICC_Clock *clock)
{
    PICC_free_atomic_int(clock->val);
    free(clock);
}



// Invariants //////////////////////////////////////////////////////////////////

/**
 * PiThread invariant.
 *
 * @inv pt != NULL
 *
 * The variables below are members of pt
 *
 * @inv status >= PICC_STATUS_RUN && status <= PICC_STATUS_BLOCKED
 * @inv enabeled_length >= 0
 * @inv enabeled_length == 0 implies enabled == NULL
 * @inv for each e in enabled e is eather true or false
 * @inv knowns != NULL
 * @inv env_length != NULL implies env != NULL
 * @inv env_length == NULL implies env == NULL
 * @inv commits != NULL
 * @inv clock != NULL
 */
void PICC_PiThread_inv(PICC_PiThread *pt)
{
    ASSERT(pt != NULL);
    ASSERT(pt->status >= PICC_STATUS_RUN);
    ASSERT(pt->status <= PICC_STATUS_BLOCKED);
    ASSERT(pt->enabled_length >= 0);
    int i;
    if (pt->enabled_length == 0) {
        ASSERT(pt->enabled == NULL);
    }
    for (i = 0; i < pt->enabled_length; ++i) {
        ASSERT(pt->enabled[i] == false || pt->enabled[i] == true);
    }
    if (pt->enabled_length == 0) {
        ASSERT(pt->enabled == NULL);
    }
    ASSERT(pt->knowns != NULL);
    if (pt->env_length == 0) {
        ASSERT(pt->env == NULL);
    } else {
        ASSERT(pt->env != NULL);
    }
    ASSERT(pt->commits != NULL);
    ASSERT(pt->clock != NULL);
}

bool PICC_process_acquire_channel(PICC_PiThread *pt, PICC_Channel *chan)
{
  while (! (PICC_try_acquire(chan->lock))) {
    pt->fuel--;
    if (pt->fuel == 0) return false;
    PICC_low_level_yield();
  }
  return true;
}


void PICC_process_yield(PICC_PiThread *pt, PICC_SchedPool *sched)
{
  pt->fuel = PICC_FUEL_INIT;
  PICC_KnownValue chan;
  PICC_KnownSet *s = PICC_knownset_forget(pt->knowns);

  // TODO: factorize this loop with the one in PICC_process_wait
  PICC_KNOWNSET_FOREACH(s, chan) {
    PICC_handle_dec_ref_count(&PICC_GET_HANDLE(&chan));
    PICC_knownset_forget_to_unknown(s, &chan);
  }
  PICC_free_knownset(s);

  PICC_ready_queue_add(sched->ready, pt);
}


void PICC_process_wait(PICC_PiThread *pt, PICC_SchedPool *sched)
{
  pt->pc = PICC_INVALID_PC;
  pt->fuel = PICC_FUEL_INIT;
  PICC_KnownValue chan;
  PICC_KnownSet *s = PICC_knownset_forget(pt->knowns);

  PICC_KNOWNSET_FOREACH(s, chan) {
    PICC_handle_dec_ref_count(&PICC_GET_HANDLE(&chan));
    PICC_knownset_forget_to_unknown(s, &chan);
  }
  PICC_free_knownset(s);

  pt->status = PICC_STATUS_WAIT;
  PICC_wait_queue_push(sched->wait, pt);
  PICC_release(pt->lock);
}


