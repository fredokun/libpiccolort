/**
 * @file sync.c
 * Synchronisation facilities.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <sync.h>

/**
 * Initializes the given mutex.
 *
 * @param mutex Mutex to initialize
 */
void PICC_init_mutex(PICC_Mutex mutex)
{
    pthread_mutex_init(&mutex, NULL);
}

/**
 * Initializes the given condition.
 *
 * @param cond Condition to initialize
 */
void PICC_init_condition(PICC_Condition cond)
{
    pthread_cond_init(&cond, NULL);
}

/**
 * Locks the given mutex.
 *
 * @param mutex Mutex to lock.
 */
void PICC_acquire(PICC_Mutex mutex)
{
    pthread_mutex_lock(&mutex);
}

/**
 * Unlocks the given mutex, fail if the mutex is already unlocked.
 *
 * @param mutex Mutex to unlock
 * @param error Error stack
 */
void PICC_release(PICC_Mutex mutex, PICC_Error *error)
{
    if (pthread_mutex_trylock(&mutex) == 0) {
        NEW_ERROR(error, ERR_MUTEX_ALREADY_UNLOCKED);
    } else {
        pthread_mutex_unlock(&mutex);
    }
}

/**
 * Waits for the condition over the given mutex.
 *
 * @param cond Mutex condition
 * @param mutex Mutex
 */
void PICC_cond_wait(PICC_Condition cond, PICC_Mutex lock)
{
    pthread_cond_wait(&cond, &lock);
}
