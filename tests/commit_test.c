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
#include <pi_thread_repr.h>
#include <commit_repr.h>
#include <value_repr.h>
#include <tools.h>

#define INIT_COMMIT(commit, pt, ch, pc) \
    commit->thread = pt; \
    commit->cont_pc = pc; \
    commit->clock = pt->clock; \
    commit->clockval = PICC_atomic_int_get(pt->clock->val); \
    commit->channel = ch;

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))

PICC_Value func(PICC_PiThread* a) { 
    printf("my eval func !\n"); 
    PICC_Value v; 
    PICC_INIT_NO_VALUE(&v);
    return v; 
};

void test_register_outcommits(PICC_Error* error)
{
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_EvalFunction eval;
    PICC_Label cont_pc;

    // check pithread, channel first
    pt = PICC_create_pithread(1, 1, 1);
    ch = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    eval = func;
    cont_pc = 10;

    PICC_register_output_commitment(pt, ch, eval, cont_pc);
}

void test_register_incommits(PICC_Error* error)
{
    int refvar;
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_Label cont_pc;

    //check pithread, channel first
    pt = PICC_create_pithread(1, 1, 1);
    ch = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    refvar = 42;
    cont_pc = 10;

    PICC_register_input_commitment(pt, ch, refvar, cont_pc);
}

void test_commitlists(PICC_Error *error)
{
    PICC_Commit *c, *c2, *c3;
    PICC_CommitListElement *clistelem;
    PICC_CommitList *clist;


    // CREATING COMMITMENTS

    c = PICC_create_commitment(error);
    c2 = PICC_create_commitment(error);
    c3 = PICC_create_commitment(error);
    ASSERT_NO_ERROR();


    // INITIALIZING COMMITMENTS
    // init c
    PICC_PiThread* pt;
    PICC_Channel *ch;
    PICC_EvalFunction eval;
    PICC_Label cont_pc;

    //check pithread, channel first
    pt = PICC_create_pithread(1, 1, 1);
    pt->clock = PICC_create_clock(error);
    ch = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    eval = func;
    cont_pc = 10;

    INIT_COMMIT(c, pt, ch, cont_pc);
    PICC_MALLOC(c->content.out, PICC_OutCommit, error);
    ASSERT_NO_ERROR();
    c->content.out->eval_func = eval;
    c->type = PICC_OUT_COMMIT;

    // init c2
    PICC_PiThread* pt2;
    PICC_Channel *ch2;
    PICC_EvalFunction eval2;
    PICC_Label cont_pc2;

    //check pithread, channel first
    pt2 = PICC_create_pithread(1, 1, 1);
    pt2->clock = PICC_create_clock(error);
    ch2 = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    eval2 = func;
    cont_pc2 = 20;

    INIT_COMMIT(c2, pt2, ch2, cont_pc2);
    PICC_MALLOC(c2->content.out, PICC_OutCommit, error);
    ASSERT_NO_ERROR();
    c2->content.out->eval_func = eval2;
    c2->type = PICC_OUT_COMMIT;

    // init c3
    int refvar;
    PICC_PiThread* pt3;
    PICC_Channel *ch3;
    PICC_Label cont_pc3;

    //check pithread, channel first
    pt3 = PICC_create_pithread(1, 1, 1);
    pt3->clock = PICC_create_clock(error);
    ch3 = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    refvar = 42;
    cont_pc3 = 30;

    INIT_COMMIT(c3, pt3, ch3, cont_pc3);
    PICC_MALLOC(c3->content.in, PICC_InCommit, error);
    ASSERT_NO_ERROR();
    c3->content.in->refvar = refvar;
    c3->type = PICC_IN_COMMIT;



    // CREATING COMMITLIST ELEMENT
    clistelem = PICC_create_commit_list_element(c, error);
    ASSERT_NO_ERROR();
    ASSERT(clistelem != NULL);

    // CREATING COMMITLIST
    clist = PICC_create_commit_list(error);
    ASSERT_NO_ERROR();
    ASSERT(clist != NULL);

    c->cont_pc = 1;
    c2->cont_pc = 2;
    c3->cont_pc = 3;


    // ADDING COMMITMENTS INTO COMMITLIST

    PICC_commit_list_add(clist, c, error);
    ASSERT_NO_ERROR();
    ASSERT(clist->head->commit == c);
    ASSERT(clist->tail->commit == c);
    ASSERT(clist->size == 1)

    PICC_commit_list_add(clist, c2, error);
    ASSERT_NO_ERROR();
    ASSERT(clist->tail->commit == c);
    ASSERT(clist->head->next->commit == c);
    ASSERT(clist->head->commit == c2);
    ASSERT(clist->size == 2);

    PICC_commit_list_add(clist, c3, error);
    ASSERT_NO_ERROR();
    ASSERT(clist->tail->commit == c);
    ASSERT(clist->head->next->commit == c2);
    ASSERT(clist->head->next->next->commit == c);
    ASSERT(clist->head->commit == c3);
    ASSERT(clist->size == 3);

    ASSERT(clist->head != NULL);
    ASSERT(clist->tail != NULL);
    ASSERT(clist->head->commit->cont_pc == 3);
    ASSERT(clist->head->next->commit->cont_pc == 2);
    ASSERT(clist->tail->commit->cont_pc == 1);


    // MODIFYING CHANNEL TO TEST FETCHING
    PICC_commit_list_add(c->channel->outcommits, c, error);
    PICC_commit_list_add(c3->channel->incommits, c3, error);


    // FETCHING COMMITMENT
    PICC_Commit *fetched_out_commit = PICC_fetch_output_commitment(c->channel);
    ASSERT(fetched_out_commit == c);

    PICC_Commit *fetched_in_commit = PICC_fetch_input_commitment(c3->channel);
    ASSERT(fetched_in_commit == c3);

    free(c);
    free(c2);
    free(c3);
    free(clistelem);
    free(clist);
}



/**
 * Runs all commit tests.
 */
void PICC_test_commit()
{
    ALLOC_ERROR(error);
    test_register_outcommits(&error);
    test_register_incommits(&error);
    test_commitlists(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

