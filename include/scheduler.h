/**
 * @file scheduler.h
 * Schedulers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue.h>
#include <sync.h>
#include <error.h>

typedef struct _PICC_SchedPool {
    struct _PICC_ReadyQueue *ready;
    struct _PICC_WaitQueue *wait;
    PICC_Mutex lock;
    PICC_Condition cond;
    int nb_slaves;
    int nb_waiting_slaves;
    bool running;
} PICC_SchedPool;

typedef struct _PICC_Args {
    PICC_Error *error;
    PICC_SchedPool *sched_pool;
} PICC_Args;

extern PICC_SchedPool *PICC_create_sched_pool(PICC_Error *error);
extern void PICC_sched_pool_slave(PICC_Args *args);
extern void PICC_sched_pool_master(PICC_SchedPool *sp, int std_gc_fuel, int quick_gc_fuel, int active_factor, PICC_Error *error);

#endif
