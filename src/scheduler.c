/**
 * @file scheduler.c
 * Schedulers.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Mickaël MENU
 */

#include <scheduler_repr.h>
#include <queue_repr.h>
#include <pi_thread_repr.h>
#include <tools.h>
#include <error.h>

#define LOCK_SCHED_POOL(sp) \
    PICC_acquire(&(sp->lock));

#define RELEASE_SCHED_POOL(sp) \
    PICC_release(&(sp->lock));

#define WAIT_SCHED_POOL(sp) \
    PICC_cond_wait(&(sp->cond), &(sp->lock));

#define SIGNAL_SCHED_POOL(sp, error) \
    PICC_cond_signal(&(sp->cond), error);

#define BROADCAST_SCHED_POOL(sp, error) \
    PICC_cond_broadcast(&(sp->cond), error);

/**
 * Creates a new scheduler.
 *
 * @param error Error stack
 * @return Created scheduler
 */
PICC_SchedPool *PICC_create_sched_pool(PICC_Error *error)
{
    PICC_ALLOC(pool, PICC_SchedPool, error) {
        ALLOC_ERROR(sub_error);
        pool->ready = PICC_create_ready_queue(&sub_error);
        pool->wait = PICC_create_wait_queue(&sub_error);
        if (HAS_ERROR(sub_error)) {
            ADD_ERROR(error, sub_error, ERR_SCHED_POOL_CREATE);
            free(pool);
            pool = NULL;
        } else {
            pool->nb_slaves = 0;
            pool->nb_waiting_slaves = 0;
            pool->running = false;
        }
        PICC_init_mutex(&(pool->lock));
        PICC_init_condition(&(pool->cond));
    }
    return pool;
}

/**
 * Creates a new set of arguments to passe to a scheduler.
 *
 * @param sp Scheduler pool
 * @param err Scheduler error stack
 * @param error Error stack
 * @return Created set of arguments
 */
PICC_Args *PICC_create_args(PICC_SchedPool *sp, PICC_Error *err, PICC_Error *error)
{
    PICC_ALLOC(args, PICC_Args, error) {
        args->sched_pool = sp;
        args->error = err;
    }
    return args;
}

/**
 * Handles the behavior of secondary real threads in scheduler pool.
 *
 * @param args Arguments containing the scheduler pool and the error stack
 */
void PICC_sched_pool_slave(PICC_Args *args)
{
    PICC_SchedPool *sched_pool = args->sched_pool;
    PICC_Error *error = args->error;

    PICC_PiThread *current;

    while(sched_pool->running) {
        while(PICC_ready_queue_size(sched_pool->ready)) {
            current = PICC_ready_queue_pop(sched_pool->ready);
            do {
                current->proc(sched_pool, current);
            } while(current->status == PICC_STATUS_CALL);

            if (current->status == PICC_STATUS_BLOCKED) // && safe_choice
                NEW_ERROR(error, ERR_DEADLOCK);
        }

        LOCK_SCHED_POOL(sched_pool);
        sched_pool->nb_waiting_slaves++;
        WAIT_SCHED_POOL(sched_pool);
        sched_pool->nb_waiting_slaves--;
        RELEASE_SCHED_POOL(sched_pool);
    }
}

/**
 * Handles the main thread in the scheduler pool after the
 * initialisation in the main entry point
 *
 * @param sp the Scheduler pool
 * @param std_gc_fuel the standard garbedge collector fuel
 * @param quick_gc_fuel the quick garbedge collector fuel
 * @param active_factor contributes somehow in the garbedge collection
 * @param error Error stack
 */
void PICC_sched_pool_master(PICC_SchedPool *sp, int std_gc_fuel, int quick_gc_fuel, int active_factor, PICC_Error *error)
{
    PICC_PiThread *current;

    while(sp->running) {
        while(PICC_ready_queue_size(sp->ready)) {
            current = PICC_ready_queue_pop(sp->ready);

            if (PICC_ready_queue_size(sp->ready) >= 1 && sp->nb_waiting_slaves > 0) {
                LOCK_SCHED_POOL(sp);
                SIGNAL_SCHED_POOL(sp, error);
                RELEASE_SCHED_POOL(sp);
            }

            do {
                current->proc(sp, current);
            } while(current->status == PICC_STATUS_CALL);

            if (current->status == PICC_STATUS_BLOCKED) // && safe_choice
                NEW_ERROR(error, ERR_DEADLOCK);
        }

        LOCK_SCHED_POOL(sp);
        if (sp->nb_waiting_slaves == sp->nb_slaves) {
            sp->running = false;
            BROADCAST_SCHED_POOL(sp, error);
        }
        RELEASE_SCHED_POOL(sp);
    }
}
