/**
 * @file commit_repr.h
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Joël HING
 */

#ifndef COMMIT_REPR_H
#define COMMIT_REPR_H

#include <commit.h>
#include <pi_thread.h>
#include <symbols.h>
#include <channel_repr.h>
#include <value.h>
#include <error.h>
#include <atomic_repr.h>

/**
 * The type of the commitment
 */
enum _PICC_CommitType {
    PICC_IN_COMMIT, /**< Designes an input commit */
    PICC_OUT_COMMIT /**< Designes an output commit */
};

/**
 * The status of the commitment
 */
enum _PICC_CommitStatus {
    PICC_CANNOT_ACQUIRE,
    PICC_VALID_COMMIT,
    PICC_INVALID_COMMIT
};

/**
 * The input commitment specific part
 */
struct _PICC_InCommit {
    /**@{*/
    int refvar; /** The reference to a variable in the pi-thread
                    referenced by this commit */
    /**@}*/
};

/**
 * The output commitment specific part
 * @inv eval_func != null
 */
struct _PICC_OutCommit {
    /**@{*/
    PICC_EvalFunction *eval_func; /** The function that evaluates the
                                    * expression passed to the channel
                                    * referenced by this commit */
    /**@}*/
};

/**
 * The commitment common part
 *
 */
struct _PICC_Commit {
    /**@{*/
    PICC_CommitType type; /**< The type of the commit */
    PICC_PiThread *thread; /** The pi-thread that has this
                                    commit */
    PICC_Clock *clock; /** The time when the commitment has
                                been made */
    PICC_AtomicInt *clockval; /**< TODO see spec */
    PICC_Label cont_pc; /**< TODO see spec */
    PICC_Channel *channel; /**< The channel of this commitment */
    /**@}*/
    /**
     * The specific part of the commitments. One of the pointers
     * below should always be NULL.
     */
    union {
        /**@{*/
        PICC_InCommit *in; /**< The in commitment specific part */
        PICC_OutCommit *out; /**< The out commitment specific part */
        /**@}*/
    } content;
};

/**
 * The type of an element of a commit list
 */
struct _PICC_CommitListElement {
    /**@{*/
    PICC_Commit *commit; /**< The referenced commit*/
    PICC_CommitListElement *next; /** A pointer to the next
                                            element or NULL if none */
    /**@}*/
};

/**
 * The commit list type
 */
struct _PICC_CommitList {
    /**@{*/
    PICC_CommitListElement *head; /**< The head of the commit list */
    PICC_CommitListElement *tail; /**< The tail of the commit list */
    int size; /**< The size of the commit list */
    /**@}*/
};

extern PICC_Commit *PICC_create_commitment(PICC_Error *error);
extern PICC_CommitList * PICC_create_commit_list(PICC_Error *error);
extern PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit, PICC_Error *error);

extern bool PICC_is_valid_commit(PICC_Commit *commit);
extern void PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *c, PICC_Error *error);

extern void PICC_Commit_inv(PICC_Commit *commit);
extern void PICC_CommitListElement_inv(PICC_CommitListElement *elem);
extern void PICC_CommitList_inv(PICC_CommitList *list);
extern void PICC_Refvar_inv(int refvar);
extern void PICC_EvalFunction_inv(PICC_EvalFunction eval);
extern void PICC_Label_inv(PICC_Label cont_pc);

#endif
