/**
 * @file main.c
 * Main for generic sets tests.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdlib.h>
#include <stdio.h>

#include <tools.h>
#include <commit.h>
#include <channel.h>
#include <set.h>

#define DEBUG

void printer(PICC_Set* commit_set)
{
    PICC_CommitL* iterator = commit_set->element.commit;

    printf("size : %d\n", commit_set->size);

    printf("-------- FIRST TEST ---------\n");

    int i;
    for(i = 0 ; i<commit_set->size ; i++)
    {
        printf("val : %d / i = %d\n", iterator->val->cont_pc, i);
        iterator = iterator->next;
    }

    iterator = commit_set->element.commit;

    printf("------- SECOND TEST ---------\n");

    for(i = 0 ; i<commit_set->size ; i++)
    {
        printf("val : %d\n", iterator->val->cont_pc);
        iterator = iterator->next;
    }
}

void test_map(void* c)
{
    PICC_Commit* cc = (PICC_Commit*)c;
    printf("1/ MAP my commit : %d\n", cc->cont_pc);
}

void test_map2(PICC_Commit* c)
{
    printf("2/ MAP my commit : %d\n", c->cont_pc);
}

int PICC_test_generic_sets()
{
    PICC_Set* commit_set = PICC_set_make(PICC_COMMIT);

    // -------- IS_EMPTY --------

    #ifdef DEBUG
    printf("is empty ? => %s\n", (PICC_set_is_empty(commit_set))?"true":"false");
    #endif

    PICC_Commit* c = PICC_create_commitment(NULL);
    c->cont_pc = 1;
    PICC_Commit* c2 = PICC_create_commitment(NULL);
    c2->cont_pc = 2;
    PICC_Commit* c3 = PICC_create_commitment(NULL);
    c3->cont_pc = 3;

    PICC_ALLOC(el, PICC_SetElement, NULL) { el->element.c = c; }
    PICC_ALLOC(el2, PICC_SetElement, NULL) { el2->element.c = c2; }
    PICC_ALLOC(el3, PICC_SetElement, NULL) { el3->element.c = c3; }

    // ---------- ADD -----------

    #ifdef DEBUG
    printf("+ adding commitments...\n");
    #endif

    PICC_SET_ADD(commit_set, el);
    PICC_SET_ADD(commit_set, el2);
    PICC_SET_ADD(commit_set, el3);

    // -------- IS_EMPTY --------

    #ifdef DEBUG
    printf("is empty ? => %s\n", (PICC_set_is_empty(commit_set))?"true":"false");
    #endif

    // ---------- MEM -----------
    // avant de tester, il faut modifier la fonction PICC_set_cmp_commit (respectivement PICC_set_cmp_knowns) pour décider quand est-ce que 2 commits sont identiques (respectivement 2 knowns).
    
    // ---------- MAP -----------
    PICC_SET_MAP(commit_set, test_map);
    PICC_SET_MAP(commit_set, (void*)(void*)test_map2);
    

    #ifdef DEBUG

    printer(commit_set);

    #endif

    free(c);
    free(c2);
    free(c3);
    free(commit_set);

    return 0;
}
