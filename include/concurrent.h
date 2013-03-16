/**
 * @file concurrent.h
 * Synchronisation facilities.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef CONCURRENT_H
#define CONCURRENT_H

#include <stdbool.h>

#include <pthread.h>
#include <error.h>

typedef pthread_mutex_t PICC_Lock;
typedef pthread_cond_t PICC_Condition;

extern PICC_Lock *PICC_create_lock(PICC_Error *error);
extern void PICC_lock_free(PICC_Lock *lock);
extern void PICC_init_lock(PICC_Lock *lock);
extern void PICC_init_condition(PICC_Condition *cond);
extern bool PICC_try_acquire(PICC_Lock *lock);
extern void PICC_acquire(PICC_Lock *lock);
extern void PICC_release(PICC_Lock *lock);
extern void PICC_cond_wait(PICC_Condition *cond, PICC_Lock *lock);
extern void PICC_cond_signal(PICC_Condition *cond, PICC_Error *error);
extern void PICC_cond_broadcast(PICC_Condition *cond, PICC_Error *error);

#endif
