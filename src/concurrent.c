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

/**
 * Initializes the given lock.
 *
 * @param lock Mutex to initialize
 */
void PICC_init_lock(PICC_Lock *lock)
{
    pthread_mutex_init(lock, NULL);
}

/**
 * Initializes the given condition.
 *
 * @param cond Condition to initialize
 */
void PICC_init_condition(PICC_Condition *cond)
{
    pthread_cond_init(cond, NULL);
}

/**
 * Locks the given lock.
 *
 * @param lock Mutex to lock.
 */
void PICC_acquire(PICC_Lock *lock)
{
    pthread_mutex_lock(lock);
}

/**
 * Tries to lock the given lock.
 *
 * @param lock Mutex to try to lock
 *
 * @return true if the lock was successfull, false otherwise
 */
bool PICC_try_acquire(PICC_Lock *lock)
{
    return pthread_mutex_trylock(lock) == 0;
}

/**
 * Unlocks the given lock, fail if the lock is already unlocked.
 *
 * @param lock Mutex to unlock
 */
void PICC_release(PICC_Lock *lock)
{
    if (pthread_mutex_trylock(lock) == 0) {
        CRASH_NEW_ERROR(ERR_MUTEX_ALREADY_UNLOCKED);
    } else {
        pthread_mutex_unlock(lock);
    }
}

/**
 * Waits for the condition over the given lock.
 *
 * @param cond Mutex condition
 * @param lock Mutex
 */
void PICC_cond_wait(PICC_Condition *cond, PICC_Lock *lock)
{
    pthread_cond_wait(cond, lock);
}

void PICC_cond_signal(PICC_Condition *cond, PICC_Error *error) {
    int status = pthread_cond_signal(cond);
    if (status) {
        NEW_ERROR(error, ERR_CONDITION_SIGNAL);
    }
}

void PICC_cond_broadcast(PICC_Condition *cond, PICC_Error *error) {
    int status = pthread_cond_broadcast(cond);
    if (status) {
        NEW_ERROR(error, ERR_CONDITION_BROADCAST);
    }
}
