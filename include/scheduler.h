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

/**
 * This type contains all the scheduler data
 */
typedef struct _PICC_SchedPool PICC_SchedPool;

/**
 * The args taken by the slave scheduler function during the posix thead
 * creation in the main entry point.
 */
typedef struct _PICC_Args PICC_Args;

#endif
