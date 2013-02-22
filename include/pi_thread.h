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

// Forward declarations
struct _PICC_SchedPool;
struct _PICC_Commit;
enum _PICC_CommitStatus;

/**
 * The status of a pi-thread
 */
typedef enum _PICC_StatusKind PICC_StatusKind;

/**
 * Try result. The result of trying to execute a thread statement
 */
typedef enum _PICC_TryResult PICC_TryResult;

/**
 * The PiThread data type
 */
typedef struct _PICC_PiThread PICC_PiThread;

/**
 * A type to represent PiThread clocks
 */
typedef struct _PICC_Clock PICC_Clock;

/**
 * The procedure type that a pi-thread executes. May use a couple of
 * labels to show where it shoud start.
 */
typedef void (PICC_PiThreadProc)(struct _PICC_SchedPool *, PICC_PiThread *);

extern PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length);
extern enum _PICC_CommitStatus PICC_can_awake(PICC_PiThread *pt, struct _PICC_Commit *commit);
extern void PICC_awake(struct _PICC_SchedPool *sched, PICC_PiThread *pt, struct _PICC_Commit *commit);
extern void PICC_low_level_yield();

#endif
