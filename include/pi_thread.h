/**
 * @file pi_thread.h
 * Pi-Threads
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Sergiu TIGANU
 */

#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>
#include <symbols.h>
#include <scheduler.h>
#include <channel.h>
#include <commit.h>
#include <value.h>
#include <sync.h>
#include <error.h>

struct _PICC_PiThread;

/**
 * @see scheduler.h
 */
struct _PICC_SchedPool;

/**
 * The procedure type that a pi-thread executes. May use a couple of
 * labels to show where it shoud start.
 */
typedef void (PICC_PiThreadProc)(struct _PICC_SchedPool *, struct _PICC_PiThread *);

/**
 * The status of a pi-thread
 */
typedef enum _PICC_StatusKind {
    PICC_STATUS_RUN, /**< A pi-thread that is ready to run */
    PICC_STATUS_CALL, /**< A pi-thread that is actually running */
    PICC_STATUS_WAIT, /**< A waiting pi-thread */
    PICC_STATUS_ENDED, /**< An ended pi-thread */
    PICC_STATUS_BLOCKED /** A blocked pi-thread (a sleeping pi-thread
                        that can't be awaked) */
} PICC_StatusKind;

/**
 * Try result. The result of trying to execute a thread statement
 */
typedef enum _PICC_TryResult {
    PICC_TRY_ENABLED, /**< The futher execution of the thred enabeled */
    PICC_TRY_DISABLED, /**< The futher execution of the thred disabled */
    PICC_TRY_COMMIT /**< A commitment has been submited */
} PICC_TryResult;

/**
 * The PiThread data type
 */
typedef struct _PICC_PiThread {
    /**@{*/
    PICC_StatusKind status; /**< The pi-thread status */
    bool *enabled; /**< TODO see spec */
    int enabled_length; /** Length of enabeled choices at the next
                            step */
    struct _PICC_KnownsSet *knowns; /**< TODO see spec */
    struct _PICC_Value **env; /**< The local pi-thread variables */
    int env_length; /**< The number of variables in the environment */
    struct _PICC_Commit *commit; /** The last commitment of the
                                    pi-thread */
    struct _PICC_CommitList *commits; /** The commitments of this
                                    pi-thread */
    PICC_PiThreadProc *proc; /** The pi-thread procedure to execute */
    PICC_Label pc; /** The label to the execution point of the
                        pi-thread procedure */
    struct _PICC_Value *val; /** The last transmited lalue over the
                                current channel */
    struct _PICC_Clock *clock; /** The pi-tjread clock. TODO see
                                spec */
    int fuel; /** Number of iterations of the pi-thread execution after
                wich the garbage collector tries to free some space */
    PICC_Mutex lock; /** The lock of the pi-thread. TODO see spec */
    /**@}*/
} PICC_PiThread;

extern PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length);
extern enum _PICC_CommitStatus PICC_can_awake(PICC_PiThread *pt, struct _PICC_Commit *commit);
extern void PICC_awake(struct _PICC_SchedPool *sched, PICC_PiThread *pt);
extern void PICC_low_level_yield();

#endif
