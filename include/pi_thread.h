/**
 * @file pi_thread.h
 * Pi-Threads
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>

typedef char * PICC_Label;
typedef void (*PICC_PiThreadProc)(PICC_SchedPool *, PICC_PiThread *);

typedef enum _PICC_StatusKind {
    PICC_STATUS_RUN,
    PICC_STATUS_CALL,
    PICC_STATUS_WAIT,
    PICC_STATUS_ENDED,
    PICC_STATUS_BLOCKED
} PICC_StatusKind;

typedef struct _PICC_PiThread {
    PICC_StatusKind status;
    bool *enabled;
    int enable_length;
    PICC_Knowns *knowns;
    PICC_Value *env;
    int env_length;
    PICC_Commit *commit;
    PICC_CommitList *commits;
    PICC_PiThreadProc *proc;
    PICC_Label pc;
    PICC_Clock *clock;
    int fuel;
    PICC_Mutex lock;
} PICC_PiThread;

extern PICC_PiThread *PICC_create_pithread();
extern bool PICC_can_awake(PICC_PiThread *pt, PICC_Commit *c);
extern void PICC_awake(PICC_SchedPool *sched, PICC_PiThread *pt);

#endif
