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
#include <commit.h>
#include <tools.h>

#define INIT_COMMIT(commit, pt, ch, pc) \
    commit->thread = pt; \
    commit->cont_pc = pc; \
    commit->clock = pt->clock; \
    commit->clockval = pt->clock->val; \
    commit->channel = ch;


PICC_Value* func(PICC_PiThread* a) { printf("my eval func !\n"); return NULL; };

void check_register_outcommits(PICC_Error* error)
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

    printf("init reg out OK\n");
    PICC_register_output_commitment(pt, ch, eval, cont_pc);

}

void check_register_incommits(PICC_Error* error)
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

}


bool check_commitlists(PICC_Error *error)
{
    PICC_Commit *c, *c2, *c3;
    PICC_CommitListElement *clistelem;
    PICC_CommitList *clist;


    // creating commitments
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
    
    
    // initializing commitments
    
    // init c
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_EvalFunction *eval;
    PICC_Label cont_pc;

    //check pithread, channel first
    pt = PICC_create_pithread(1, 1);
    ch = PICC_create_channel(error);
    eval = func;
    cont_pc = 10;
    
    INIT_COMMIT(c, pt, ch, cont_pc);
    PICC_MALLOC(c->content.out, PICC_OutCommit, error);
    c->content.out->eval_func = eval;
    c->type = PICC_OUT_COMMIT;
    
    // init c2
    PICC_PiThread* pt2;
    PICC_Channel *ch2;
    PICC_EvalFunction *eval2;
    PICC_Label cont_pc2;

    //check pithread, channel first
    pt2 = PICC_create_pithread(1, 1);
    ch2 = PICC_create_channel(error);
    eval2 = func;
    cont_pc2 = 20;
    
    INIT_COMMIT(c2, pt2, ch2, cont_pc2);
    PICC_MALLOC(c2->content.out, PICC_OutCommit, error);
    c2->content.out->eval_func = eval2;
    c2->type = PICC_OUT_COMMIT;
    
    // init c3
    int refvar;
    PICC_PiThread* pt3;
    PICC_Channel *ch3;
    PICC_Label cont_pc3;

    //check pithread, channel first
    pt3 = PICC_create_pithread(1, 1);
    ch3 = PICC_create_channel(error);
    refvar = 42;
    cont_pc3 = 30;
    
    INIT_COMMIT(c3, pt3, ch3, cont_pc3);
    PICC_MALLOC(c3->content.in, PICC_InCommit, error);
    c3->content.in->refvar = refvar;
    c3->type = PICC_IN_COMMIT;

    

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

void PICC_test_commit()
{
    ALLOC_ERROR(error);
    
    check_register_outcommits(&error);
    printf("reg out OK\n");
    check_register_incommits(&error);
    printf("reg in OK\n");
    ASSERT(check_commitlists(&error));
    printf("commit list OK\n");
    
    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

