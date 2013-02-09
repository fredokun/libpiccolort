/**
 * @file pi_thread.c
 * Pi-Threads
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#include <pi_thread.h>
#include <tools.h>
#include <value.h>

/**
 * Creates a new PiThread with given environment and knowns set length.
 *
 * @param env_length Size of the environment
 * @param knowns_length Size of the knowns set
 * @return Created PiThread
 */
PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length)
{
    PICC_ALLOC_CRASH(thread, PICC_PiThread) {
        ALLOC_ERROR(sub_error);
        thread->knowns = PICC_create_knowns_set(knowns_length, &sub_error);
        if (HAS_ERROR(sub_error)) {
            CRASH(&sub_error);
        } else {
            PICC_ALLOC_N_CRASH(env, PICC_Value, env_length) {
                thread->clock = PICC_create_clock(&sub_error);
                if (HAS_ERROR(sub_error)) {
                    CRASH(&sub_error);
                } else {
                    thread->env = env;
                    thread->env_length = env_length;
                    thread->enabled = NULL;
                    thread->enabled_length = 0;
                    thread->commit = NULL;
                    thread->commits = NULL;
                    thread->proc = NULL;
                    thread->pc = PICC_DEFAULT_ENTRY_LABEL;
                    thread->fuel = PICC_FUEL_INIT;
                    thread->val = NULL;
                    PICC_init_mutex(&(thread->lock));
                }
            }
        }
    }
    return thread;
}

/**
 * Returns whether a PiThread can be awaken with the given commit.
 *
 * @param pt PiThread to check
 * @param commit Commitment
 * @return Whether the PiThread can be awaken with given commit
 */
enum _PICC_CommitStatus PICC_can_awake(PICC_PiThread *pt, PICC_Commit *commit)
{
    if (!PICC_try_acquire(&(pt->lock))) {
        return PICC_CANNOT_ACQUIRE;
    }
    if (commit->clock != pt->clock || commit->clockval != commit->clock->val) {
        PICC_release(&(pt->lock));
        return PICC_INVALID_COMMIT;
    }
    if (pt->clock->val == PICC_CLOCK_MAX_INT) {
        PICC_reclaim_clock(pt->clock);
        pt->clock = NULL;
        ALLOC_ERROR(error);
        pt->clock = PICC_create_clock(&error);
        if (HAS_ERROR(error)) {
            CRASH(&error);
        }
    }
    CRASH_NEW_ERROR(ERR_NOT_IMPLEMENTED);
    return PICC_STATUS_ENDED;
}

/**
 * Awakes a PiThread in the given scheduler.
 *
 * @param sched Scheduler
 * @param pt PiThread to be awaken
 */
void PICC_awake(PICC_SchedPool *sched, PICC_PiThread *pt)
{
    CRASH_NEW_ERROR(ERR_NOT_IMPLEMENTED);
}

/**
 * ??????????????
 */
void PICC_low_level_yield()
{
    CRASH_NEW_ERROR(ERR_NOT_IMPLEMENTED);
}
