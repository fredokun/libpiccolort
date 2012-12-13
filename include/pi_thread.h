/**
 * @file pi_thread.h
 * Pi-Threads
 *
 * This project is released under MIT License.
 */

#ifndef PI_THREAD_H
#define PI_THREAD_H

#include <stdbool.h>
#include <pthread.h>
#include <definitions.h>
#include <queue.h>
#include <entry.h>
#include <error.h>

struct PICC_AtomicInt {
    PICC_Mutex lock;
    int value;
};

struct PICC_AtomicBoolean {
    PICC_Mutex lock;
    bool value;
};

struct PICC_Clock {
    PICC_AtomicInt *val;
};

struct PICC_InCommit {
    int refvar;
};

struct PICC_OutCommit {
    PICC_EvalFunction *eval_func;
};

struct PICC_Commit
{
    PICC_CommitType type;
    PICC_PiThread *thread;
    PICC_Clock *clock;
    PICC_AtomicInt *clockval;
    int cont_pc;
    PICC_Channel *channel;

    union
    {
        PICC_InCommit *in;
        PICC_OutCommit *out;
    } content;
};

struct PICC_CommitListElement
{
    PICC_Commit *commit;
    struct PICC_CommitListElement *next;
};

struct PICC_CommitList
{
    PICC_CommitListElement *head;
    PICC_CommitListElement *tail;
    int size;
};

struct PICC_SchedPool {
    PICC_ReadyQueue *ready;
    PICC_WaitQueue *wait;
    PICC_Mutex lock;
    PICC_Condition cond;
    int nb_slaves;
    int nb_waiting_slaves;
    bool running;
};

struct PICC_PiThread {
    PICC_StatusKind status;
    bool* enabled;
    int enable_length;
    PICC_Knowns * knowns;
    PICC_Value* env;
    int env_length;
    PICC_Commit *commit;
    PICC_CommitList *commits;
    PICC_PiThreadProc proc;
    PICC_Label pc;
    PICC_Clock *clock;
    int fuel;
    PICC_Mutex lock;
};

struct PICC_Channel {
    PICC_Commit* incommits;
    PICC_Commit* outcommits;
    int global_rc;
    PICC_AtomicBoolean lock;
};

struct PICC_Value {
    PICC_ValueKind kind;
    union {
        int as_int;
        double as_float;
        char *as_string;
        bool as_bool;
        PICC_Channel *as_channel;
    } content;
};

struct PICC_Knowns {
    PICC_Channel *channel;
    PICC_KnownsState state;
};

#endif // PI_THREAD_H
