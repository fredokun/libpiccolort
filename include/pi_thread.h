#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>
#include <pthread.h>

typedef struct PIT_SchedPool PIT_SchedPool;
typedef struct PIT_PiThread PIT_PiThread;
typedef struct PIT_Channel PIT_Channel;
typedef struct PIT_Commit PIT_Commit;
typedef struct PIT_InCommit PIT_InCommit;
typedef struct PIT_OutCommit PIT_OutCommit;
typedef struct PIT_Clock PIT_Clock;
typedef struct PIT_Value PIT_Value;
typedef struct PIT_AtomicBoolean PIT_AtomicBoolean;
typedef struct PIT_AtomicInt PIT_AtomicInt;
typedef struct PIT_Known PIT_Known;

typedef pthread_mutex_t PIT_Mutex;
typedef struct PIT_Condition *PIT_Condition;

typedef char *PIT_Label;
typedef void (*PIT_Function)(void);
typedef PIT_Value (*PIT_EvalFunction)(PIT_PiThread);

typedef struct PIT_ReadyQueue PIT_ReadyQueue;
typedef struct PIT_WaitQueue PIT_WaitQueue;

typedef struct PIT_Queue PIT_Queue;
typedef struct PIT_QueueCell PIT_QueueCell;

typedef enum
{
	IN_COMMIT,
	OUT_COMMIT
} PIT_CommitType;

typedef enum 
{
  STATUS_RUN,
  STATUS_CALL,
  STATUS_WAIT,
  STATUS_ENDED
} PIT_StatusKind;

typedef enum 
{
  INT_VAL,
  FLOAT_VAL,
  STRING_VAL,
  BOOL_VAL,
  CHANNEL_VAL,
} PIT_ValueKind;

typedef enum
{
  UNKNOWN,
  KNOWN,
  FORGET
} PIT_KnownsState;

struct PIT_AtomicBoolean {
	pthread_mutex_t lock;
	bool value;	
};

struct PIT_AtomicInt {
	pthread_mutex_t lock;
	int value;	
};

struct PIT_Clock {
  PIT_AtomicInt val;
};

struct PIT_InCommit 
{
  int refvar;
};

struct PIT_OutCommit 
{
  PIT_EvalFunction eval_func;
};

struct PIT_Commit
{
	PIT_CommitType type;
	PIT_PiThread *thread;
	PIT_Clock clock;
	int clockval;
	int cont_pc;
	PIT_Channel *channel;

	union
	{
		PIT_InCommit in;
		PIT_OutCommit out;
	} content;
};

struct PIT_QueueCell {
  PIT_PiThread *thread;
  PIT_QueueCell *next;
};

struct PIT_Queue {
  PIT_QueueCell *head;
  PIT_QueueCell *tail;
  int size;
};

struct PIT_ReadyQueue {
  PIT_Queue q;
  PIT_Mutex lock;
};

struct PIT_WaitQueue {
  PIT_Queue active;
  PIT_Queue old;
  PIT_Mutex lock;
};

struct PIT_Known
{
  PIT_Channel *pchannel;
  PIT_KnownsState state;
};

struct PIT_SchedPool 
{
  PIT_ReadyQueue ready;
  PIT_WaitQueue wait;
  PIT_Mutex lock;
  PIT_Condition cond;
  int nb_slaves;
  int nb_waiting_slaves;
};

struct PIT_PiThread 
{
  PIT_StatusKind status;
  bool* enable;
  int enable_length;
  PIT_Known* knowns;
  PIT_Value* env;
  int env_length;
  PIT_Commit commit;
  PIT_Commit* commits;
  PIT_Function proc;
  PIT_Label pc;
  int fuel;
  PIT_Mutex lock;
};

struct PIT_Channel 
{
  PIT_Commit* incommits;
  PIT_Commit* outcommits;
  int global_rc;
  PIT_AtomicBoolean lock;
};

struct PIT_Value 
{
  PIT_ValueKind kind;
  union {
    int as_int;
    double as_float;
    char *as_string;
    bool as_bool;
    PIT_Channel *as_channel;
  } content;
};

#endif // PI_THREAD_H
