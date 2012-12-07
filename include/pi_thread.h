#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>
#include <pthread.h>
#include <definitions.h>

struct PIT_AtomicInt {
	PIT_Mutex lock;
	int value;	
};

struct PIT_AtomicBoolean {
	PIT_Mutex lock;
	bool value;	
};

struct PIT_Clock {
	PIT_AtomicInt val;
};

struct PIT_InCommit {
	int refvar;
};

struct PIT_OutCommit {
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

struct PIT_SchedPool {
	PIT_ReadyQueue *ready;
	PIT_WaitQueue *wait;
	PIT_Mutex lock;
	PIT_Condition *cond;
	int nb_slaves;
	int nb_waiting_slaves;
};

struct PIT_PiThread {
	PIT_StatusKind status;
	bool* enable;
	int enable_length;
	PIT_Knowns * knowns;
	PIT_Value* env;
	int env_length;
	PIT_Commit commit;
	PIT_Commit* commits;
	PIT_Function proc;
	PIT_Label pc;
	PIT_Clock clock;
	int fuel;
	PIT_Mutex lock;
};

struct PIT_Channel {
	PIT_Commit* incommits;
	PIT_Commit* outcommits;
	int global_rc;
	PIT_AtomicBoolean lock;
};

struct PIT_Value {
	PIT_ValueKind kind;
	union {
		int as_int;
		double as_float;
		char *as_string;
		bool as_bool;
		PIT_Channel *as_channel;
	} content;
};

struct PIT_Knowns {
	PIT_Channel *channel;
	PIT_KnownsState state;
};

#endif // PI_THREAD_H
