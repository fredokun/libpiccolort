/**
 * @file commit.c
 * Commits.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Joel HING
 */

#include <commit.h>
#include <tools.h>

#define LOCK_CLOCK(commit) \
    PICC_acquire(&(commit->thread->clock->val->lock));

#define RELEASE_CLOCK(commit) \
    PICC_release(&(commit->thread->clock->val->lock));

#define INIT_COMMIT(commit, pt, ch, pc) \
    commit->thread = pt; \
    commit->cont_pc = pc; \
    commit->clock = pt->clock; \
    commit->clockval = pt->clock->val; \
    commit->channel = ch;

/**
 * Creates and returns a commitment.
 *
 * @post commit != null
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

    #ifdef CONTRACT
        // post
        ASSERT(commit != NULL);
    #endif

    return commit;
}

/**
 * Creates a new commit list.
 *
 * @post clist != NULL
 * @post clist->head = NULL
 * @post clist->tail = NULL
 * @post clist->size = 0
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
    
     #ifdef CONTRACT        
        //post
        ASSERT(clist != NULL);
        ASSERT(clist->head == NULL);
        ASSERT(clist->tail == NULL);
        ASSERT(clist->size == 0);
        
    #endif
   
    return clist;
}

/**
 * Creates a new  element of commit list.
 * 
 * @pre commit != NULL
 * 
 * @post clist_elem != NULL
 * @post clist_elem->commit = commit
 * @post clist_elem->next = NULL
 *
 * @param commit Commit associated
 * @param error Error stack
 * @return Created commit list element
 */
PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit, PICC_Error *error)
{
	#ifdef CONTRACT

        // pre
        ASSERT(commit != NULL);

    #endif
	
    PICC_ALLOC(clist_elem, PICC_CommitListElement, error) {
        clist_elem->commit = commit;
        clist_elem->next = NULL;
    }
    
    #ifdef CONTRACT
		
        //post
        ASSERT(clist_elem != NULL);
        ASSERT(clist_elem->commit == commit);
        ASSERT(clist_elem->next == NULL);
        
    #endif
    
    return clist_elem;
}

/**
 * Registers an output commit with given PiThread and channel.
 *
 * @pre pt != null && ch != null && eval != null &&  cont_pc >= 0 
 *
 * @post pt->commits->size = pt->commits->size_at_pre + 1
 * @post pt->commits->tail = commit
 * @post commit->type = PICC_OUT_COMMIT
 * @post commit->content.out = out
 * @post commit->thread = pt
 * @post commit->channel = ch
 * @post commit->cont_pc = cont_pc
 * @post out->eval_func = eval
 *
 * @param pt PiThread associated with the commit
 * @param ch Channel used to create the commit
 * @param eval Evaluation function
 * @param cont_pc Programm counter
 */
void PICC_register_output_commitment(PICC_PiThread *pt, PICC_Channel *ch, PICC_EvalFunction *eval, PICC_Label cont_pc)
{
    #ifdef CONTRACT
        // inv
        // PICC_PiThread_inv(pt);
        // PICC_Channel_inv(ch);
        PICC_EvalFunction_inv(eval);
        PICC_Label_inv(cont_pc);

        // pre
        ASSERT(pt != NULL);
        ASSERT(ch != NULL);
        ASSERT(eval != NULL);
        ASSERT(cont_pc >= 0);

        // captures
        int size_at_pre = pt->commits->size;
    #endif

    ALLOC_ERROR(sub_error);
    PICC_Commit *commit = PICC_create_commitment(&sub_error);
    if (HAS_ERROR(sub_error)) {
        ADD_ERROR(&sub_error, sub_error, ERR_REGISTER_OUT_COMMIT);
    }
    else
    {
        INIT_COMMIT(commit, pt, ch, cont_pc);
        PICC_MALLOC(commit->content.out, PICC_OutCommit, &sub_error);
        commit->content.out->eval_func = eval;
        commit->type = PICC_OUT_COMMIT;

        ALLOC_ERROR(add_error);
        PICC_commit_list_add(pt->commits, commit, &add_error);
        if (HAS_ERROR(add_error)) {
            ADD_ERROR(&sub_error, add_error, ERR_REGISTER_IN_COMMIT);
            free(commit);
        }
    }
    if (HAS_ERROR(sub_error))
        CRASH(&sub_error);

    #ifdef CONTRACT
        // inv
        // PICC_PiThread_inv(pt);
        // PICC_Channel_inv(ch);
        PICC_EvalFunction_inv(eval);
        PICC_Label_inv(cont_pc);
        
        //post
        ASSERT(pt->commits->size == (size_at_pre + 1));
        ASSERT(pt->commits->head->commit->type == PICC_OUT_COMMIT);
        ASSERT(pt->commits->head->commit->content.out->eval_func == eval);
        ASSERT(pt->commits->head->commit->thread == pt);
        ASSERT(pt->commits->head->commit->channel == ch);
        ASSERT(pt->commits->head->commit->cont_pc == cont_pc);
    #endif
}
/**
 * Registers an input commit with given PiThread and channel.
 *
 *
 * @pre pt != null && ch != null && cont_pc >= 0 
 *
 * @post pt->commits->size(PICC_register_input_commitment(pt)) = pt->commits->size(pt) + 1
 * @post pt->commits->tail = commit
 * @post commit->type = PICC_IN_COMMIT
 * @post commit->content.in = in
 * @post commit->thread = pt
 * @post commit->channel = ch
 * @post commit->cont_pc = cont_pc
 * @post in->refvar = refvar
 *
 * @param pt PiThread associated with the commit
 * @param ch Channel used to create the commit
 * @param refvar Index of the var used to create the input commit
 * @param cont_pc Program counter
 */
void PICC_register_input_commitment(PICC_PiThread *pt, PICC_Channel *ch, int refvar, PICC_Label cont_pc)
{
	#ifdef CONTRACT
        // inv
        // PICC_PiThread_inv(pt);
        // PICC_Channel_inv(ch);
        PICC_Refvar_inv(refvar);
        PICC_Label_inv(cont_pc);

        //pre
		ASSERT(pt != NULL);
		ASSERT(ch != NULL);
		ASSERT(cont_pc >= 0);

        // captures
        int size_at_pre = pt->commits->size;
    #endif
	
    ALLOC_ERROR(sub_error);
    PICC_Commit *commit = PICC_create_commitment(&sub_error);
    if (HAS_ERROR(sub_error)) {
        ADD_ERROR(&sub_error, sub_error, ERR_REGISTER_IN_COMMIT);
    } else {
        INIT_COMMIT(commit, pt, ch, cont_pc);
        PICC_MALLOC(commit->content.in, PICC_InCommit, &sub_error);
        commit->content.in->refvar = refvar;
        commit->type = PICC_IN_COMMIT;

        ALLOC_ERROR(add_error);
        PICC_commit_list_add(pt->commits, commit, &add_error);
        if (HAS_ERROR(add_error)) {
            ADD_ERROR(&sub_error, add_error, ERR_REGISTER_IN_COMMIT);
            free(commit);
        }
    }
    
    if (HAS_ERROR(sub_error))
        CRASH(&sub_error);
        
    #ifdef CONTRACT
        // inv
        // PICC_PiThread_inv(pt);
        // PICC_Channel_inv(ch);
        PICC_Refvar_inv(refvar);
        PICC_Label_inv(cont_pc);

        
        //post
		ASSERT(pt->commits->size == (size_at_pre + 1));
		ASSERT(pt->commits->head->commit->type == PICC_IN_COMMIT);
		ASSERT(pt->commits->head->commit->content.in->refvar == refvar);
		ASSERT(pt->commits->head->commit->thread == pt);
		ASSERT(pt->commits->head->commit->channel == ch);
		ASSERT(pt->commits->head->commit->cont_pc == cont_pc);
    #endif
        
}

/**
 * Verifies if the given commit is valid.
 *
 * @pre commit != null
 *
 * @post  if (commit->clock == commit->thread->clock && commit->clockval == commit->thread->clock->val) valid = true else valid = false
 *
 * @param commit Commit to validate
 * @return Whether the commit is valid
 */
bool PICC_is_valid_commit(PICC_Commit *commit)
{
	#ifdef CONTRACT
        // inv
        PICC_Commit_inv(commit);


		// pre
		ASSERT(commit != NULL);
		
    #endif
    
    bool valid = false;
    LOCK_CLOCK(commit);
    if (commit->clock == commit->thread->clock
    && commit->clockval == commit->thread->clock->val)
        valid = true;
    RELEASE_CLOCK(commit);
    
    
    #ifdef CONTRACT
        // inv
        PICC_Commit_inv(commit);

        
        // post
        if ((commit->clock == commit->thread->clock) && (commit->clockval == commit->thread->clock->val)){
			ASSERT(valid == true); 
		}
        else {
			ASSERT(valid == false);
		}
	
    #endif
    
    return valid;
    
}


/**
 * Adds the given element at the end of the commit list.
 *
 * @pre clist != NULL
 * @pre commit != NULL
 * 
 * @post clist->size = clist_at_pre->size + 1
 * @post clist->tail->commit = commit
 * @post clist->tail->next = NULL
 * 
 * @param clist Commit list
 * @param commit Commit to add
 * @param error Error stack
 */
void PICC_commit_list_add(PICC_CommitList *clist, PICC_Commit *commit, PICC_Error *error)
{
	#ifdef CONTRACT
        // inv
        PICC_CommitList_inv(clist);
        PICC_Commit_inv(commit);

		// pre
		ASSERT(clist != NULL);
		ASSERT(commit != NULL);
		
		// capture
		int size_at_pre = clist->size;
		
    #endif
	
    ALLOC_ERROR(create_error);
    PICC_CommitListElement *clist_elem = PICC_create_commit_list_element(commit, &create_error);
    if (HAS_ERROR(create_error)) {
        ADD_ERROR(error, create_error, ERR_ADD_COMMIT_TO_LIST);
    } else {
        if(clist->head != NULL && clist->tail != NULL){            
            clist->tail->next = clist_elem;
            clist->tail = clist_elem;
            clist->size++;
        }
        else{
            clist->head = clist_elem;
            clist->tail = clist_elem;
            clist->size++;
        }
    }
    
    #ifdef CONTRACT
		// inv
        PICC_CommitList_inv(clist);
		PICC_Commit_inv(commit);
        
        //post
		ASSERT(clist->size == size_at_pre + 1);
		ASSERT(clist->tail->commit == commit);
		ASSERT(clist->tail->next == NULL);
		
    #endif
}

/**
 * Returns whether a commit list is empty.
 *
 * @pre clist != NULL
 * 
 * @post if clist->size = 0 , res = true else res = false
 * 
 * @param clist Commit list to check
 * @return Whether the commit list is empty.
 */
bool PICC_commit_list_is_empty(PICC_CommitList *clist)
{
	#ifdef CONTRACT
        // inv
        PICC_CommitList_inv(clist);


		// pre
		ASSERT(clist != NULL);
		
    #endif
	
    ASSERT(clist != NULL);
    bool res = clist->size == 0;
    
    
    #ifdef CONTRACT
		// inv
        PICC_CommitList_inv(clist);

        
        //post
		if(clist-> size == 0){
			ASSERT(res);
		}
		else{
			ASSERT(!res);
		}
		
    #endif
    
    return res;
}

/**
 * Fetch the first element of the commit list.
 *
 * @pre clist != NULL
 * @pre clist->size > 0
 * 
 * @post fetched = clist_at_pre->head->commit
 * @post clist->size = clist_at_pre->size - 1
 * @post clist->head = clist_at_pre->head->next
 * 
 * @param clist Commit list
 * @return first element of the commit list
 */
PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *clist)
{
	#ifdef CONTRACT
        // inv
        PICC_CommitList_inv(clist);


		// pre
		ASSERT(clist != NULL);
		ASSERT(clist->size > 0);
		
		// capture
		PICC_CommitListElement *head_at_pre = clist->head;
		int size_at_pre = clist->size;
		
    #endif
	
    PICC_CommitListElement *commit_list_element = clist->head;
    clist->head = commit_list_element->next;
    clist->size--;
    PICC_Commit *fetched  = commit_list_element->commit;
    commit_list_element->next = NULL;
    commit_list_element->commit = NULL;
    free(commit_list_element);
    
    #ifdef CONTRACT
		// inv
        PICC_CommitList_inv(clist);

        
        //post
		ASSERT(fetched == head_at_pre->commit);		
		ASSERT(clist->size == size_at_pre - 1);
		ASSERT(clist->head == head_at_pre->next);
		
    #endif
    
    return fetched;
}

/**
 * Fetches the first element of the input commitList from a channel.
 *
 * @pre ch != null
 *
 * @post if(c->incommits->size > 0 && PICC_is_valid_commit(PICC_commit_list_fetch(c->incommits))) PICC_fetch_input_commitment(c) =  PICC_commit_list_fetch(c->incommits)
 * @post if(c->incommits->size = 0 || !(PICC_is_valid_commit(PICC_commit_list_fetch(c->incommits)))) PICC_fetch_input_commitment(c) = null
 *
 * @param ch Channel to fetch the commit from
 * @return Fetched commit
 */
PICC_Commit *PICC_fetch_input_commitment(PICC_Channel *ch)
{
	#ifdef CONTRACT
        // inv
        //PICC_Channel_inv(ch);


		// pre
		ASSERT(ch != NULL);
		
    #endif
	
    PICC_Commit *fetched = NULL;
    ALLOC_ERROR(alloc_error);
    PICC_ALLOC(current, PICC_Commit, &alloc_error) {
        current = PICC_commit_list_fetch(ch->incommits);
        while (current != NULL) {
            if (PICC_is_valid_commit(current)) {
                return current;
            }
    	    current = PICC_commit_list_fetch(ch->incommits);
        }
    }

    if (HAS_ERROR(alloc_error))
        CRASH(&alloc_error);

	#ifdef CONTRACT
		// inv
        // PICC_Channel_inv(ch);

        
        //post
        if((ch->incommits->size > 0) && (PICC_is_valid_commit(PICC_commit_list_fetch(ch->incommits)))){
			ASSERT(fetched ==  PICC_commit_list_fetch(ch->incommits));
		}
		
		if((ch->incommits->size = 0) || (!(PICC_is_valid_commit(PICC_commit_list_fetch(ch->incommits))))){
			ASSERT(fetched == NULL);
		}


    #endif

    return fetched;
}

/**
 * Fetches he first element of the output commitList from a channel
 *
 * @pre ch != null
 *
 * @post if(c->outcommits->size > 0 && PICC_is_valid_commit(PICC_commit_list_fetch(c->outcommits))) PICC_fetch_output_commitment(c) =  PICC_commit_list_fetch(c->outcommits)
 * @post if(c->outcommits->size = 0 || !(PICC_is_valid_commit(PICC_commit_list_fetch(c->outcommits)))) PICC_fetch_output_commitment(c) = null
 *
 * @param ch Channel to fetch the commit from
 * @return Fetched commit
 */
PICC_Commit *PICC_fetch_output_commitment(PICC_Channel *ch)
{
	#ifdef CONTRACT
        // inv
        //PICC_Channel_inv(ch);


		// pre
		ASSERT(ch != NULL);
		
    #endif
    
    PICC_Commit *fetched = NULL;
    ALLOC_ERROR(alloc_error);
    PICC_ALLOC(current, PICC_Commit, &alloc_error)
    {
        current = PICC_commit_list_fetch(ch->outcommits);
        while (current != NULL) {
            if (PICC_is_valid_commit(current)) {
                fetched = current;
                break;
            }
            current = PICC_commit_list_fetch(ch->outcommits);
        }
    }

    if (HAS_ERROR(alloc_error))
        CRASH(&alloc_error);
        

	#ifdef CONTRACT
		// inv
        // PICC_Channel_inv(ch);

        
        //post
        if((ch->outcommits->size > 0) && (PICC_is_valid_commit(PICC_commit_list_fetch(ch->outcommits)))){
			ASSERT(fetched ==  PICC_commit_list_fetch(ch->outcommits));
		}
		
		if((ch->outcommits->size = 0) || (!(PICC_is_valid_commit(PICC_commit_list_fetch(ch->outcommits))))){
			ASSERT(fetched == NULL);
		}


    #endif
    

    return fetched;
}




// INVARIANTS CHECKING FUNCTIONS ///////////////////////////////////////////////////////////////////


/**
 * @brief Checks commit invariant.
 *
 * @inv thread != null && clock != null && clockval  != null && channel  != null && content  != null
 * @inv cont_pc > 0
 */
void PICC_Commit_inv(PICC_Commit *commit)
{
    ASSERT(commit->thread != NULL);
    ASSERT(commit->clock != NULL);
    ASSERT(commit->clockval != NULL);
    ASSERT(commit->channel != NULL);

    if (commit->type == PICC_IN_COMMIT) {
        ASSERT(commit->content.in != NULL);
    } else {
        ASSERT(commit->content.out != NULL);
    }
    ASSERT(commit->cont_pc > 0);
}

/**
 * Checks commit list element invariant.
 * 
 * @inv elem->commit != NULL
 */
void PICC_CommitListElement_inv(PICC_CommitListElement *elem)
{
	ASSERT(elem->commit != NULL);
}

/**
 * Checks commit list invariant.
 * 
 * @inv if list->size = 0, list->head = NULL && list->tail = NULL
 * @inv if list->size = 1, list->head = list->tail
 * @inv if list->size > 1, list->head != list->tail
 */
void PICC_CommitList_inv(PICC_CommitList *list)
{
	if(list->size == 0){
		ASSERT(list->head == NULL && list->tail == NULL);
	}
	
	if(list->size == 1){
		ASSERT(list->head == list->tail);
	}
	
	if(list->size > 1){
		ASSERT(list->head != list->tail);
	}
}
/**
 * Checks refvar invariant.
 * 
 * @inv refvar > 0
 */
void PICC_Refvar_inv(int refvar)
{
	
}

/**
 * Checks evalFunction invariant.
 * 
 * @inv eval !=  NULL
 */
void PICC_EvalFunction_inv(PICC_EvalFunction eval)
{
	
}

/**
 * Checks Label invariant.
 * 
 * @inv cont_pc > -1
 */
void PICC_Label_inv(PICC_Label cont_pc)
{
	
}
