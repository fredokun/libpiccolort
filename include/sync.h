/**
 * @file sync.h
 * Synchronisation facilities.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef SYNC_H
#define SYNC_H

#include <stdbool.h>

#include <pthread.h>
#include <error.h>

typedef pthread_mutex_t PICC_Mutex;
typedef pthread_cond_t PICC_Condition;

extern void PICC_init_mutex(PICC_Mutex *mutex);
extern void PICC_init_condition(PICC_Condition *cond);
extern bool PICC_try_acquire(PICC_Mutex *mutex);
extern void PICC_acquire(PICC_Mutex *mutex);
extern void PICC_release(PICC_Mutex *mutex);
extern void PICC_cond_wait(PICC_Condition *cond, PICC_Mutex *lock);
extern void PICC_cond_signal(PICC_Condition *cond, PICC_Error *error);
extern void PICC_cond_broadcast(PICC_Condition *cond, PICC_Error *error);
#endif
