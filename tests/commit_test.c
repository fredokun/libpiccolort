/**
 * @file commit_tests.c
 * File that contains all the necesseray tests to check the behavior of all functions of runtime.c
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Joel HING
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <pi_thread.h>

PICC_Value* func(PICC_PiThread* a) { printf("my eval func !\n"); return NULL; };

bool check_register_outcommits(PICC_Error* error)
{
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_EvalFunction *eval;
    PICC_Label cont_pc;

    //check pithread, channel first
    pt = PICC_create_pithread(1, 1);
    ch = PICC_create_channel(error);
    eval = func;
    cont_pc = 10;

    PICC_register_output_commitment(pt, ch, eval, cont_pc);

    return true;
}

bool check_register_incommits(PICC_Error* error)
{
    int refvar;
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_Label cont_pc;

    //check pithread, channel first
    pt = PICC_create_pithread(1, 1);
    ch = PICC_create_channel(error);
    refvar = 42;
    cont_pc = 10;

    PICC_register_input_commitment(pt, ch, refvar, cont_pc);

    return true;
}

/**
 * Test : Commitments \n
 * PICC_CommitList *PICC_create_commit_list(); \n
 * PICC_CommitListElement *PICC_create_commit_list_element();
 *
 * @return boolean true if it works else false
 */
bool check_commitlists(PICC_Error *error)
{
    PICC_Commit *c, *c2, *c3;
    PICC_CommitListElement *clistelem;
    PICC_CommitList *clist;

    ALLOC_ERROR(create_error);
    c = PICC_create_commitment(&create_error);
    if (HAS_ERROR(create_error)) {
        FORWARD_ERROR(error, create_error);
        return false;
    }

    c2 = PICC_create_commitment(&create_error);
    if (HAS_ERROR(create_error)) {
        FORWARD_ERROR(error, create_error);
        return false;
    }

    c3 = PICC_create_commitment(&create_error);
    if (HAS_ERROR(create_error)) {
        FORWARD_ERROR(error, create_error);
        return false;
    }


    //is_valid ?
    if(!PICC_is_valid_commit(c))
    {
        NEW_ERROR(error, ERR_INVALID_COMMIT);
        return false;
    }

    clistelem = PICC_create_commit_list_element(c, &create_error);
    if (HAS_ERROR(create_error)) {
        FORWARD_ERROR(error, create_error);
        return false;
    } else if(clistelem == NULL) {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMITLISTELEM);
        return false;
    }

    clist = PICC_create_commit_list(&create_error);
    if (HAS_ERROR(create_error)) {
        FORWARD_ERROR(error, create_error);
        return false;

    } else if(clist == NULL) {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMITLIST);
        return false;
    }

    clist->head = clistelem;
    clist->tail = clistelem;

    c->cont_pc = 1;
    c2->cont_pc = 2;
    c3->cont_pc = 3;

    ALLOC_ERROR(add_error);
    PICC_commit_list_add(clist, c, &add_error);
    PICC_commit_list_add(clist, c2, &add_error);
    PICC_commit_list_add(clist, c3, &add_error);
    if (HAS_ERROR(add_error)) {
        FORWARD_ERROR(error, add_error);
        return false;
    }

    if(clist->head == NULL || clist->tail == NULL)
    {
        NEW_ERROR(error, ERR_NULLPOINTER_COMMITLISTELEM);
        return false;
    }

    if(clist->head->commit->cont_pc != 1)
    {
        NEW_ERROR(error, ERR_INVALID_VALUE);
        return false;
    }

    if(clist->head->next->commit->cont_pc != 2)
    {
        NEW_ERROR(error, ERR_INVALID_VALUE);
        return false;
    }

    if(clist->tail->commit->cont_pc != 3)
    {
        NEW_ERROR(error, ERR_INVALID_VALUE);
        return false;
    }

    free(c);
    free(c2);
    free(c3);
    free(clistelem);
    free(clist);

    return true;
}

void PICC_test_commit(){
    ALLOC_ERROR(error);
    
    ASSERT(check_register_outcommits(&error));
    ASSERT(check_register_incommits(&error));
    ASSERT(check_commitlists(&error));
}

