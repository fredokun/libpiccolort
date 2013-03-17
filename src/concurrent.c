/**
 * @file concurrent.c
 * Synchronisation facilities.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#include <concurrent.h>
#include <pthread.h>
#include <tools.h>
#include <stdio.h>
/**
 * Creates a new lock.
 *
 * @return Allocated lock
 */
PICC_Lock *PICC_create_lock(PICC_Error *error)
{
    PICC_ALLOC(lock, PICC_Lock, error) {
        PICC_init_lock(lock);
    }
    return lock;
}

void PICC_lock_free(PICC_Lock *l){
    free(l);
}

/**
 * Initializes the given lock.
 *
 * @pre lock != null
 * @param lock Mutex to initialize
 */
void PICC_init_lock(PICC_Lock *lock)
{
    #ifdef CONTRACT_PRE
        ASSERT(lock != NULL);
    #endif

    pthread_mutex_init(lock, NULL);
}

/**
 * Initializes the given condition.
 *
 * @pre cond != null
 * @param cond Condition to initialize
 */
void PICC_init_condition(PICC_Condition *cond)
{
    #ifdef CONTRACT_PRE
        ASSERT(cond != NULL);
    #endif

    pthread_cond_init(cond, NULL);
}

/**
 * Locks the given lock.
 *
 * @pre lock != null
 * @param lock Mutex to lock.
 */
void PICC_acquire(PICC_Lock *lock)
{
    #ifdef CONTRACT_PRE
        ASSERT(lock != NULL);
    #endif
    pthread_mutex_lock(lock);
}

/**
 * Tries to lock the given lock.
 *
 * @pre lock != null
 * @param lock Mutex to try to lock
 * @return true if the lock was successfull, false otherwise
 */
bool PICC_try_acquire(PICC_Lock *lock)
{
    #ifdef CONTRACT_PRE
        ASSERT(lock != NULL);
    #endif

    return pthread_mutex_trylock(lock) == 0;
}

/**
 * Unlocks the given lock, fail if the lock is already unlocked.
 *
 * @pre lock != null
 * @param lock Mutex to unlock
 */
void PICC_release(PICC_Lock *lock)
{
    #ifdef CONTRACT_PRE
        ASSERT(lock != NULL);
    #endif

    if (pthread_mutex_trylock(lock) == 0) {
        CRASH_NEW_ERROR(ERR_MUTEX_ALREADY_UNLOCKED);
    } else {
        pthread_mutex_unlock(lock);
    }
}

/**
 * Waits for the condition over the given lock.
 *
 * @pre cond != null
 * @pre lock != null
 * @param cond Mutex condition
 * @param lock Mutex
 */
void PICC_cond_wait(PICC_Condition *cond, PICC_Lock *lock)
{
    #ifdef CONTRACT_PRE
        ASSERT(cond != NULL);
        ASSERT(lock != NULL);
    #endif

    pthread_cond_wait(cond, lock);
}

/**
 * Sends a signal for the given condition.
 *
 * @pre cond != null
 * @param cond Mutex condition
 * @param error Error stack
 */
void PICC_cond_signal(PICC_Condition *cond, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        ASSERT(cond != NULL);
    #endif

    int status = pthread_cond_signal(cond);
    if (status) {
        NEW_ERROR(error, ERR_CONDITION_SIGNAL);
    }
}

/**
 * Broadcasts a signal for the given condition.
 *
 * @pre cond != null
 * @param cond Mutex condition
 * @param error Error stack
 */
void PICC_cond_broadcast(PICC_Condition *cond, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        ASSERT(cond != NULL);
    #endif

    int status = pthread_cond_broadcast(cond);
    if (status) {
        NEW_ERROR(error, ERR_CONDITION_BROADCAST);
    }
}
