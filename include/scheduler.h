/**
 * @file scheduler.h
 * Schedulers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue.h>
#include <sync.h>
#include <error.h>

/**
 * This type contains all the scheduler data
 */
typedef struct _PICC_SchedPool {
    /**@{*/
    struct _PICC_ReadyQueue *ready; /** The queue that contains the 
                                        pi-threads ready tuo run */
    struct _PICC_WaitQueue *wait; /** The queue that contains the 
                                    waiting or blocked pi-threads */
    PICC_Mutex lock; /**< The scheduler lock. TODO see spec */
    PICC_Condition cond; /** The scheduler condition. Used to 
                            synchronise the running posix threads. */
    int nb_slaves; /** The number of running posix threads in the 
                        schedpool. */
    int nb_waiting_slaves; /**< The number of waiting posix threads. */
    bool running; /**< Specifies if the scheduler is actually running */
    /**@}*/
} PICC_SchedPool;

/**
 * The args taken by the slave scheduler function during the posix thead 
 * creation in the main entry point.
 */
typedef struct _PICC_Args {
    /**@{*/
    PICC_Error *error; /**< The errors list */
    PICC_SchedPool *sched_pool; /**< a pointer to the scheduler data */
    /**@}*/
} PICC_Args;

extern PICC_SchedPool *PICC_create_sched_pool(PICC_Error *error);
extern PICC_Args *PICC_create_args(PICC_SchedPool *sp, PICC_Error *err, PICC_Error *error);
extern void PICC_sched_pool_slave(PICC_Args *args);
extern void PICC_sched_pool_master(PICC_SchedPool *sp, int std_gc_fuel, int quick_gc_fuel, int active_factor, PICC_Error *error);

#endif
