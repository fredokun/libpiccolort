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
#include <channel.h>
#include <commit.h>
#include <value_repr.h>
#include <concurrent.h>
#include <atomic.h>
#include <error.h>

static const PICC_Label PICC_DEFAULT_ENTRY_LABEL = 0;

/**
 * The maximum number of iterations a thread can execute without being scheduled.
 */
static const int PICC_FUEL_INIT = 358;

/**
 * Invalid position in the program counter.
 */
static const int PICC_INVALID_PC = -1;

/**
 *  The maximum value a PiThread clock may have
 */
static const int PICC_CLOCK_MAX_INT = 1000;

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
 * A type to represent PiThread clocks
 */
struct _PICC_Clock {
    /**@{*/
    PICC_AtomicInt *val; /** Contains the timestamp when the clock has
                           * been stopped. TODO a function that puts a
                           * timestamp in a clock */
    /**@{*/
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
    /* PICC_KnownSet *chans; /\** The channels curently acquired by this  */
    /*                           thread *\/ */
    PICC_KnownSet *knowns; /** The channels known by this
                                        thread */
    PICC_Value *env; /**< The local pi-thread variables */
    int env_length; /**< The number of variables in the environment */
    PICC_Commit *commit; /** The last commitment of the
                                    pi-thread */
    PICC_CommitList *commits; /** The commitments of this
                                    pi-thread */
    PICC_PiThreadProc *proc; /** The pi-thread procedure to execute */
    PICC_Label pc; /** The label to the execution point of the
                        pi-thread procedure */
    PICC_Value val; /** Buffer used as a worskpace in the generated code  */
    PICC_Clock *clock; /** The pi-thread clock. TODO see
                                spec */
    int fuel; /** Number of iterations of the pi-thread execution after
                wich it goes to the end of the ready queue */
    PICC_Lock *lock; /** The lock of the pi-thread. TODO see spec */
    /**@}*/
};

extern PICC_Clock *PICC_create_clock(PICC_Error *error);
extern void PICC_reclaim_clock(PICC_Clock *clock);

extern void PICC_PiThread_inv(PICC_PiThread *pt);
extern void PICC_reclaim_pi_thread(PICC_PiThread *pt);

#endif
