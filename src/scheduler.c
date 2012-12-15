/**
 * @file scheduler.c
 * Schedulers.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Mickaël MENU
 */

#include <scheduler.h>
#include <tools.h>

#define LOCK_SCHED_POOL(sp) \
    PICC_acquire(sp->lock);

#define RELEASE_SCHED_POOL(sp) \
    PICC_release(sp->lock, NULL);

#define WAIT_SCHED_POOL(sp) \
    PICC_cond_wait(sp->cond, sp->lock);

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
	}
	return pool;
}

/**
 * Handles the behavior of a scheduler pool.
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
 * ????????????????????
 *
 * @param sp ????????????????
 * @param std_gc_fuel ???????????????
 * @param quick_gc_fuel ?????????????
 * @param active_factor ?????????????
 * @param error Error stack
 */
void PICC_sched_pool_master(PICC_SchedPool *sp, int std_gc_fuel, int quick_gc_fuel, int active_factor, PICC_Error *error)
{
	NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
}
