#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef struct PIT_ReadyQueue PIT_ReadyQueue;
typedef struct PIT_WaitQueue PIT_WaitQueue;
typedef struct PIT_QueueCell PIT_QueueCell;
typedef struct PIT_SchedPool PIT_SchedPool;
typedef struct PIT_PiThread PIT_PiThread;
typedef struct PIT_Channel PIT_Channel;
typedef struct PIT_Commit PIT_Commit;
typedef struct PIT_InCommit PIT_InCommit;
typedef struct PIT_OutCommit PIT_OutCommit;
typedef struct PIT_Clock PIT_Clock;
typedef struct PIT_Value PIT_Value;
typedef struct PIT_AtomicBoolean PIT_AtomicBoolean;

typedef pthread_mutex_t PIT_Mutex;
typedef struct PIT_Condition PIT_Condition;
typedef struct PIT_Knowns PIT_Knowns;
typedef PIT_Knowns* PIT_KnownsSet;


typedef char *PIT_Label;
typedef void (*PIT_Function)(void);
typedef PIT_Value (*PIT_EvalFunction)(PIT_PiThread);
typedef int PIT_AtomicInt;

typedef enum
{
	IN_COMMIT,
	OUT_COMMIT
} PIT_CommitType;

typedef enum {
	STATUS_RUN,
	STATUS_CALL,
	STATUS_WAIT,
	STATUS_ENDED
} PIT_StatusKind;

typedef enum {
	INT_VAL,
	FLOAT_VAL,
	STRING_VAL,
	BOOL_VAL,
	CHANNEL_VAL,
} PIT_ValueKind;

typedef enum {
	UNKNOWN,
	KNOWN,
	FORGET
} PIT_KnownsState;

typedef enum PIT_ErrorId PIT_ErrorId;
typedef struct PIT_Error PIT_Error;

// TO BE MODIFIED WHEN YOU ADD AN ERROR ////////////////////////////////////////

// Number of errors defined.
#define NB_ERRORS 3

// List of all errors defined.
enum PIT_ErrorId {
	ERR_WRONG_ARG = 1,
	ERR_FOO_FAILED,
	ERR_BAR_FAILED
	/* ... */
};

#endif