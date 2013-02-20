/**
 * @file sync.c
 * Synchronisation facilities.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#include <sync.h>

/**
 * Initializes the given mutex.
 *
 * @param mutex Mutex to initialize
 */
void PICC_init_mutex(PICC_Mutex *mutex)
{
    pthread_mutex_init(mutex, NULL);
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
 * Locks the given mutex.
 *
 * @param mutex Mutex to lock.
 */
void PICC_acquire(PICC_Mutex *mutex)
{
    pthread_mutex_lock(mutex);
}

/**
 * Tries to lock the given mutex.
 *
 * @param mutex Mutex to try to lock
 * 
 * @return true if the lock was successfull, false otherwise
 */
bool PICC_try_acquire(PICC_Mutex *mutex)
{
    return pthread_mutex_trylock(mutex) == 0;
}

/**
 * Unlocks the given mutex, fail if the mutex is already unlocked.
 *
 * @param mutex Mutex to unlock
 */
void PICC_release(PICC_Mutex *mutex)
{
    if (pthread_mutex_trylock(mutex) == 0) {
        CRASH_NEW_ERROR(ERR_MUTEX_ALREADY_UNLOCKED);
    } else {
        pthread_mutex_unlock(mutex);
    }
}

/**
 * Waits for the condition over the given mutex.
 *
 * @param cond Mutex condition
 * @param mutex Mutex
 */
void PICC_cond_wait(PICC_Condition *cond, PICC_Mutex *lock)
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
