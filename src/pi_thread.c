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

/**
 * Creates a new PiThread with given environment and knowns set length.
 *
 * @param env_length Size of the environment
 * @param knowns_length Size of the knowns set
 * @param error Error stack
 * @return Created PiThread
 */
PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length, PICC_Error *error)
{
    // TO BE COMPLETED...
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return NULL;

    // PICC_ALLOC(thread, PICC_PiThread, error) {
    //  ALLOC_ERROR(sub_error);
    //  thread->knowns = PICC_create_knowns_set(knowns_length, &sub_error);
    //  if (HAS_ERROR(sub_error)) {
    //      ADD_ERROR(error, sub_error, ERR_THREAD_CREATE);
    //  } else {
    //      thread->fuel = PICC_FUEL_INIT;
    //  }
    // }

 //    return new_thread;
}

/**
 * Returns whether a PiThread can be awaken with the given commit.
 *
 * @param pt PiThread to check
 * @param commit Commitment
 * @return Whether the PiThread can be awaken with given commit
 */
enum _PICC_CommitStatus PICC_can_awake(PICC_PiThread *pt, PICC_Commit *commit, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return false;
}

/**
 * Awakes a PiThread in the given scheduler.
 *
 * @param sched Scheduler
 * @param pt PiThread to be awaken
 */
void PICC_awake(PICC_SchedPool *sched, PICC_PiThread *pt, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
}
