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
#include <scheduler.h>
#include <channel.h>
#include <commit.h>
#include <value.h>
#include <sync.h>
#include <error.h>

static const int PICC_FUEL_INIT = 358;

struct _PICC_PiThread;
struct _PICC_SchedPool;
typedef int PICC_Label;
typedef void (PICC_PiThreadProc)(struct _PICC_SchedPool *, struct _PICC_PiThread *);

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
    int enabled_length;
    struct _PICC_KnownsSet *knowns;
    PICC_Value **env;
    int env_length;
    struct _PICC_Commit *commit;
    struct _PICC_CommitList *commits;
    PICC_PiThreadProc *proc;
    PICC_Label pc;
    PICC_Value *val;
    PICC_Clock *clock;
    int fuel;
    PICC_Mutex lock;
} PICC_PiThread;

extern PICC_PiThread *PICC_create_pithread(int env_length, int knowns_length, PICC_Error *error);
extern bool PICC_can_awake(PICC_PiThread *pt, struct _PICC_Commit *commit, PICC_Error *error);
extern void PICC_awake(struct _PICC_SchedPool *sched, PICC_PiThread *pt, PICC_Error *error);

#endif
