/**
 * @file commit.c
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <commit.h>
#include <tools.h>

#define LOCK_CLOCK(commit) \
    PICC_acquire(commit->thread->clock->val->lock);

#define RELEASE_CLOCK(commit) \
    PICC_release(commit->thread->clock->val->lock, NULL);

/**
 * Creates and returns a commitment.
 *
 * @return Created commitment
 */
PICC_Commit *PICC_create_commitment(PICC_Error *error)
{
	PICC_ALLOC(commit, PICC_Commit, error) {
	    commit->thread = NULL;
	    commit->clock = NULL;
	    commit->channel = NULL;
	}

    return commit;
}

/**
 * Registers an output commit with given PiThread and channel.
 *
 * @param pt PiThread associated with the commit
 * @param ch Channel used to create the commit
 * @param eval Evaluation function
 * @param cont_pc Programm counter
 * @param error Error stack
 */
void PICC_register_out_commitment(PICC_PiThread *pt, PICC_Channel *ch, PICC_EvalFunction *eval, PICC_Label cont_pc, PICC_Error *error)
{
	PICC_ALLOC(out, PICC_OutCommit, error) {
	    out->eval_func = eval;
	    ALLOC_ERROR(sub_error);
	    PICC_Commit *commit = PICC_create_commitment(&sub_error);
	    if (HAS_ERROR(sub_error)) {
	    	ADD_ERROR(error, sub_error, ERR_REGISTER_OUT_COMMIT);
	    	free(out);
	    } else {
		    commit->content.out = out;
		    commit->thread = pt;
		    commit->cont_pc = cont_pc;
		    commit->type = PICC_OUT_COMMIT;
		    commit->clock = pt->clock;
		    commit->clockval = pt->clock->val;
		    commit->channel = ch;
		    PICC_commit_list_add(pt->commits, commit);
		}
	}
}

/**
 * Registers an input commit with given PiThread and channel.
 *
 * @param pt PiThread associated with the commit
 * @param ch Channel used to create the commit
 * @param refvar Index of the var used to create the input commit
 * @param cont_pc Program counter
 * @param error Error stack
 */
void PICC_register_in_commitment(PICC_PiThread *pt, PICC_Channel *ch, int refvar, int cont_pc, PICC_Error *error)
{
	PICC_ALLOC(in, PICC_InCommit, error) {
    	in->refvar = refvar;
	    ALLOC_ERROR(sub_error);
	    PICC_Commit *commit = PICC_create_commitment(&sub_error);
	    if (HAS_ERROR(sub_error)) {
	    	ADD_ERROR(error, sub_error, ERR_REGISTER_IN_COMMIT);
	    	free(in);
	    } else {
		    commit->content.in = in;
		    commit->thread = pt;
		    commit->cont_pc = cont_pc;
		    commit->type = PICC_IN_COMMIT;
		    commit->clock = pt->clock;
		    commit->clockval = pt->clock->val;
		    commit->channel = ch;
		    PICC_commit_list_add(pt->commits ,commit);
		}
	}
}

/**
 * Verifies if the given commit is valid.
 *
 * @param commit Commit to validate
 * @return Whether the commit is valid
 */
bool PICC_is_valid_commit(PICC_Commit *commit, PICC_Error *error)
{
	bool valid = false;
    LOCK_CLOCK(commit);
    if (commit->clock == commit->thread->clock
    && commit->clockval == commit->thread->clock->val)
        valid = true;
    RELEASE_CLOCK(commit);
    return valid;
}

/**
 * ????????????????????
 *
 * @param clist ???????
 * @return ????????????
 */
PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return NULL;
}

/**
 * Fetches a commit from a channel.
 *
 * @param ch Channel to fetch the commit from
 * @return Fetched commit
 */
PICC_Commit *PICC_fetch_commitment(PICC_Channel *ch, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return NULL;
}
