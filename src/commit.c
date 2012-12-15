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

#define INIT_COMMIT(commit, pt, ch, pc) \
	commit->thread = pt; \
	commit->cont_pc = pc; \
	commit->clock = pt->clock; \
	commit->clockval = pt->clock->val; \
	commit->channel = ch;

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
 * Creates a new commit list.
 *
 * @param error Error stack
 * @return Created commit list
 */
PICC_CommitList *PICC_create_commit_list(PICC_Error *error)
{
	PICC_ALLOC(clist, PICC_CommitList, error) {
	    clist->head = NULL;
	    clist->tail = NULL;
	    clist->size = 0;
	}
	return clist;
}

/**
 * Creates a new  element of commit list.
 *
 * @param commit Commit associated
 * @param error Error stack
 * @return Created commit list element
 */
PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit, PICC_Error *error)
{
	PICC_ALLOC(clist_elem, PICC_CommitListElement, error) {
	    clist_elem->commit = commit;
	    clist_elem->next = NULL;
	}
    return clist_elem;
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
void PICC_register_output_commitment(PICC_PiThread *pt, PICC_Channel *ch, PICC_EvalFunction *eval, PICC_Label cont_pc, PICC_Error *error)
{
	PICC_ALLOC(out, PICC_OutCommit, error) {
	    out->eval_func = eval;
	    ALLOC_ERROR(sub_error);
	    PICC_Commit *commit = PICC_create_commitment(&sub_error);
	    if (HAS_ERROR(sub_error)) {
	    	ADD_ERROR(error, sub_error, ERR_REGISTER_OUT_COMMIT);
	    	free(out);
	    } else {
	    	INIT_COMMIT(commit, pt, ch, cont_pc);
			commit->content.out = out;
		    commit->type = PICC_OUT_COMMIT;

		    ALLOC_ERROR(add_error);
		    PICC_commit_list_add(pt->commits, commit, &add_error);
		    if (HAS_ERROR(add_error)) {
		    	ADD_ERROR(error, add_error, ERR_REGISTER_IN_COMMIT);
		    	free(commit);
		    	free(out);
		    }
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
void PICC_register_input_commitment(PICC_PiThread *pt, PICC_Channel *ch, int refvar, int cont_pc, PICC_Error *error)
{
	PICC_ALLOC(in, PICC_InCommit, error) {
    	in->refvar = refvar;
	    ALLOC_ERROR(sub_error);
	    PICC_Commit *commit = PICC_create_commitment(&sub_error);
	    if (HAS_ERROR(sub_error)) {
	    	ADD_ERROR(error, sub_error, ERR_REGISTER_IN_COMMIT);
	    	free(in);
	    } else {
	    	INIT_COMMIT(commit, pt, ch, cont_pc);
		    commit->content.in = in;
		    commit->type = PICC_IN_COMMIT;

		    ALLOC_ERROR(add_error);
		    PICC_commit_list_add(pt->commits, commit, &add_error);
		    if (HAS_ERROR(add_error)) {
		    	ADD_ERROR(error, add_error, ERR_REGISTER_IN_COMMIT);
		    	free(commit);
		    	free(in);
		    }
		}
	}
}

/**
 * Verifies if the given commit is valid.
 *
 * @param commit Commit to validate
 * @return Whether the commit is valid
 */
bool PICC_is_valid_commit(PICC_Commit *commit)
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
 * Adds the given element at the end of the commit list.
 *
 * @param clist Commit list
 * @param commit Commit to add
 * @param error Error stack
 */
void PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *commit, PICC_Error *error)
{
	ALLOC_ERROR(create_error);
    PICC_CommitListElement *clist_elem = PICC_create_commit_list_element(commit, &create_error);
    if (HAS_ERROR(create_error)) {
    	ADD_ERROR(error, create_error, ERR_ADD_COMMIT_TO_LIST);
    } else {
	    clist->tail->next = clist_elem;
	    clist->tail = clist_elem;
	    clist->size++;
	}
}

/**
 * ????????????????????
 *
 * @param clist ???????
 * @return ????????????
 */
PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist)
{
    PICC_CommitListElement *commit_list_element = clist->head;
    clist->head = commit_list_element->next;
    clist->size--;
    commit_list_element->next = NULL;
    return commit_list_element->commit;
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
