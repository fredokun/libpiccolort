/**
 * @file pi_thread_repr.h
 * Pi-Threads
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Sergiu TIGANU
 */

#ifndef PI_THREAD_REPR_H
#define PI_THREAD_REPR_H

#include <stdbool.h>
#include <pi_thread.h>
#include <scheduler.h>
#include <symbols.h>
#include <channel.h>
#include <value.h>
#include <concurrent.h>
#include <error.h>

/**
 * The status of a pi-thread
 */
enum _PICC_StatusKind {
    PICC_STATUS_RUN, /**< A pi-thread that is ready to run */
    PICC_STATUS_CALL, /**< A pi-thread that is actually running */
    PICC_STATUS_WAIT, /**< A waiting pi-thread */
    PICC_STATUS_ENDED, /**< An ended pi-thread */
    PICC_STATUS_BLOCKED /** A blocked pi-thread (a sleeping pi-thread
                        that can't be awaked) */
};

/**
 * Try result. The result of trying to execute a thread statement
 */
enum _PICC_TryResult {
    PICC_TRY_ENABLED, /**< The futher execution of the thred enabeled */
    PICC_TRY_DISABLED, /**< The futher execution of the thred disabled */
    PICC_TRY_COMMIT /**< A commitment has been submited */
};

/**
 * The PiThread data type
 */
struct _PICC_PiThread {
    /**@{*/
    PICC_StatusKind status; /**< The pi-thread status */
    bool *enabled; /**< In case of a guarded choice tells if a choice i
                        may or may not be followed */
    int enabled_length; /** Length of enabeled choices at the next
                            step */
    struct _PICC_KnownsSet *knowns; /** The channels known by this
                                        thread */
    struct _PICC_Value **env; /**< The local pi-thread variables */
    int env_length; /**< The number of variables in the environment */
    struct _PICC_Commit *commit; /** The last commitment of the
                                    pi-thread */
    struct _PICC_CommitList *commits; /** The commitments of this
                                    pi-thread */
    PICC_PiThreadProc *proc; /** The pi-thread procedure to execute */
    PICC_Label pc; /** The label to the execution point of the
                        pi-thread procedure */
    struct _PICC_Value *val; /** The last transmited value over the
                                current channel */
    struct _PICC_Clock *clock; /** The pi-thread clock. TODO see
                                spec */
    int fuel; /** Number of iterations of the pi-thread execution after
                wich it goes to the end of the ready queue */
    PICC_Lock lock; /** The lock of the pi-thread. TODO see spec */
    /**@}*/
};

extern void PICC_PiThread_inv(PICC_PiThread *pt);

#endif
