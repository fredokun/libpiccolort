/**
 * @file commit.h
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Joël HING
 */

#ifndef COMMIT_H
#define COMMIT_H

#include <stdbool.h>
#include <pi_thread.h>

// Forward declarations
struct _PICC_Channel;
struct _PICC_Value;

/**
 * TODO see spec
 */
typedef struct _PICC_Value (*PICC_EvalFunction)(struct _PICC_PiThread *);
//typedef struct _PICC_Value *(*PICC_EvalFunction)(struct _PICC_PiThread *);

/**
 * The type of the commitment
 */
typedef enum _PICC_CommitType PICC_CommitType;

/**
 * The status of the commitment
 */
typedef enum _PICC_CommitStatus PICC_CommitStatus;

/**
 * The input commitment specific part
 */
typedef struct _PICC_InCommit PICC_InCommit;

/**
 * The output commitment specific part
 * @inv eval_func != null
 */
typedef struct _PICC_OutCommit PICC_OutCommit;

/**
 * The commitment common part
 *
 */
typedef struct _PICC_Commit PICC_Commit;

/**
 * The type of an element of a commit list
 */
typedef struct _PICC_CommitListElement PICC_CommitListElement;

/**
 * The commit list type
 */
typedef struct _PICC_CommitList PICC_CommitList;

extern void PICC_register_output_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, PICC_EvalFunction eval, PICC_Label cont_pc);
extern void PICC_register_input_commitment(struct _PICC_PiThread *p, struct _PICC_Channel *ch, int refvar, PICC_Label cont_pc);

extern bool PICC_commit_list_is_empty(PICC_CommitList *clist);
extern PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist);
extern PICC_Commit *PICC_fetch_input_commitment(struct _PICC_Channel *ch);
extern PICC_Commit *PICC_fetch_output_commitment(struct _PICC_Channel *ch);

extern PICC_EvalFunction PICC_eval_func_of_output_commitment(PICC_Commit *ch);

#define CHANNEL_OF_CHANNEL_VALUE(val) (PICC_Channel*)((PICC_ChannelValue *)(val))->data
#define OUTCOMMITS_LIST_OF_VALUE(val) (CHANNEL_OF_CHANNEL_VALUE(val))->outcommits
#define INCOMMITS_LIST_OF_VALUE(val) (CHANNEL_OF_CHANNEL_VALUE(val))->incommits


#define PICC_FETCH_OUTPUT_COMMIT_FROM_VALUE(ch)			\
    PICC_fetch_output_commitment(CHANNEL_OF_CHANNEL_VALUE(ch))

#define PICC_FETCH_INPUT_COMMIT_FROM_VALUE(ch)			\
    PICC_fetch_input_commitment(CHANNEL_OF_CHANNEL_VALUE(ch))

#define PICC_REGISTER_OUTPUT_COMMITMENT_FROM_VALUE(p, ch, eval, cont)	\
    PICC_register_output_commitment(p, CHANNEL_OF_CHANNEL_VALUE(ch),	\
				    eval, cont)

#define PICC_REGISTER_INPUT_COMMITMENT_FROM_VALUE(p, ch, rv, cont)	\
    PICC_register_input_commitment(p, CHANNEL_OF_CHANNEL_VALUE(ch),	\
				   rv, cont)

#endif
