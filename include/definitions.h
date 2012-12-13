/**
 * @file definitions.h
 * File that contains typedef + all enums of runtime.h
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Mickaël MENU
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <pthread.h>

typedef struct PICC_Queue PICC_Queue;
typedef struct PICC_ReadyQueue PICC_ReadyQueue;
typedef struct PICC_WaitQueue PICC_WaitQueue;
typedef struct PICC_QueueCell PICC_QueueCell;

typedef struct PICC_SchedPool PICC_SchedPool;
typedef struct PICC_PiThread PICC_PiThread;

typedef struct PICC_Channel PICC_Channel;

typedef struct PICC_Commit PICC_Commit;
typedef struct PICC_CommitList PICC_CommitList;
typedef struct PICC_CommitListElement PICC_CommitListElement;
typedef struct PICC_InCommit PICC_InCommit;
typedef struct PICC_OutCommit PICC_OutCommit;

typedef struct PICC_Clock PICC_Clock;
typedef struct PICC_Value PICC_Value;
typedef struct PICC_AtomicBoolean PICC_AtomicBoolean;
typedef struct PICC_AtomicInt PICC_AtomicInt;

typedef pthread_mutex_t PICC_Mutex;
typedef pthread_cond_t PICC_Condition;
typedef struct PICC_Knowns PICC_Knowns;
typedef PICC_Knowns* PICC_KnownsSet;

typedef char *PICC_Label;
typedef void (*PICC_PiThreadProc) (PICC_SchedPool, PICC_PiThread);
typedef PICC_Value* (*PICC_EvalFunction)(PICC_PiThread);

typedef struct PICC_Args PICC_Args;

typedef enum
{
    PICC_IN_COMMIT,
    PICC_OUT_COMMIT
} PICC_CommitType;

typedef enum {
    PICC_STATUS_RUN,
    PICC_STATUS_CALL,
    PICC_STATUS_WAIT,
    PICC_STATUS_ENDED,
    PICC_STATUS_BLOCKED
} PICC_StatusKind;

typedef enum {
    PICC_INT_VAL,
    PICC_FLOAT_VAL,
    PICC_STRING_VAL,
    PICC_BOOL_VAL,
    PICC_CHANNEL_VAL,
} PICC_ValueKind;

typedef enum {
    PICC_UNKNOWN,
    PICC_KNOWN,
    PICC_FORGET
} PICC_KnownsState;

#endif
