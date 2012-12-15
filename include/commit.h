/**
 * @file commit.h
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef COMMIT_H
#define COMMIT_H

#include <pi_thread.h>
#include <channel.h>
#include <value.h>
#include <error.h>

struct _PICC_PiThread;
typedef struct _PICC_Value *(PICC_EvalFunction)(struct _PICC_PiThread *);

typedef enum _PICC_CommitType {
    PICC_IN_COMMIT,
    PICC_OUT_COMMIT
} PICC_CommitType;

typedef struct _PICC_InCommit {
    int refvar;
} PICC_InCommit;

typedef struct _PICC_OutCommit {
    PICC_EvalFunction *eval_func;
} PICC_OutCommit;

typedef struct _PICC_Commit {
    PICC_CommitType type;
    struct _PICC_PiThread *thread;
    struct _PICC_Clock *clock;
    struct _PICC_Value *clockval;
    int cont_pc;
    struct _PICC_Channel *channel;
    union {
        PICC_InCommit *in;
        PICC_OutCommit *out;
    } content;
} PICC_Commit;

typedef struct _PICC_CommitListElement {
    PICC_Commit *commit;
    struct _PICC_CommitListElement *next;
} PICC_CommitListElement;

typedef struct _PICC_CommitList {
    PICC_CommitListElement *head;
    PICC_CommitListElement *tail;
    int size;
} PICC_CommitList;

extern PICC_Commit *PICC_create_commitment(PICC_Error *error);
extern PICC_CommitList *PICC_create_commit_list(PICC_Error *error);
extern PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit, PICC_Error *error);

extern void PICC_register_ouput_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, PICC_EvalFunction *eval, int cont_pc, PICC_Error *error);
extern void PICC_register_input_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, int refvar, int cont_pc, PICC_Error *error);

extern bool PICC_is_valid_commit(PICC_Commit *commit, PICC_Error *error);
extern void PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *c, PICC_Error *error);
extern PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist);
extern PICC_Commit *PICC_fetch_commitment(struct _PICC_Channel *ch, PICC_Error *error);

#endif
