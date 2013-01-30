/**
 * @file commit.h
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#ifndef COMMIT_H
#define COMMIT_H

#include <pi_thread.h>
#include <channel.h>
#include <value.h>
#include <error.h>

/**
 * @see pi_thread.h
 */
struct _PICC_PiThread;

/**
 * TODO see spec
 */
typedef struct _PICC_Value *(PICC_EvalFunction)(struct _PICC_PiThread *);

/**
 * The type of the commitment
 */
typedef enum _PICC_CommitType {
    PICC_IN_COMMIT, /**< Designes an input commit */
    PICC_OUT_COMMIT /**< Designes an output commit */
} PICC_CommitType;

/**
 * The status of the commitment
 */
typedef enum _PICC_CommitStatus {
    PICC_CANNOT_ACQUIRE,
    PICC_VALID_COMMIT,
    PICC_INVALID_COMMIT
} PICC_CommitStatus;

/**
 * The input commitment specific part
 */
typedef struct _PICC_InCommit {
    /**@{*/
    int refvar; /** The reference to a variable in the pi-thread
                    referenced by this commit */
    /**@}*/
} PICC_InCommit;

/**
 * The output commitment specific part
 * @inv eval_func != null
 */
typedef struct _PICC_OutCommit {
    /**@{*/
    PICC_EvalFunction *eval_func; /** The function that evaluates the
                                    * expression passed to the channel
                                    * referenced by this commit */
    /**@}*/
} PICC_OutCommit;

/**
 * The commitment common part
 * @inv type != null && thread != null && clock != null && clockval  != null && channel  != null && content  != null
 * @inv cont_pc > 0
 * 
 */
typedef struct _PICC_Commit {
    /**@{*/
    PICC_CommitType type; /**< The type of the commit */
    struct _PICC_PiThread *thread; /** The pi-thread that has this
                                    commit */
    struct _PICC_Clock *clock; /** The time when the commitment has
                                been made */
    struct _PICC_Value *clockval; /**< TODO see spec */
    int cont_pc; /**< TODO see spec */
    struct _PICC_Channel *channel; /**< The channel of this commitment */
    /**@}*/
    /**
     * @name The specific pârt of the commitments. One of the pointers
     * below should always be NULL.
     */
    union {
        /**@{*/
        PICC_InCommit *in; /**< The in commitment specific part */
        PICC_OutCommit *out; /**< The out commitment specific part */
        /**@}*/
    } content;
} PICC_Commit;

/**
 * The type of an element of a commit list
 */
typedef struct _PICC_CommitListElement {
    /**@{*/
    PICC_Commit *commit; /**< The refferenced commit*/
    struct _PICC_CommitListElement *next; /** A pointer to the next
                                            element or NULL if none */
    /**@}*/
} PICC_CommitListElement;

/**
 * The commit list type
 */
typedef struct _PICC_CommitList {
    /**@{*/
    PICC_CommitListElement *head; /**< The head of the commit list */
    PICC_CommitListElement *tail; /**< The tail of the commit list */
    int size; /**< The size of the commit list */
    /**@}*/
} PICC_CommitList;

extern PICC_Commit *PICC_create_commitment(PICC_Error *error);
extern PICC_CommitList *PICC_create_commit_list(PICC_Error *error);
extern PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit, PICC_Error *error);

extern void PICC_register_ouput_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, PICC_EvalFunction *eval, PICC_Label cont_pc, PICC_Error *error);
extern void PICC_register_input_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, int refvar, PICC_Label cont_pc, PICC_Error *error);

extern bool PICC_is_valid_commit(PICC_Commit *commit);
extern void PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *c, PICC_Error *error);
extern PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist);
extern PICC_Commit *PICC_fetch_commitment(struct _PICC_Channel *ch, PICC_Error *error);

#endif
