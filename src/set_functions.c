/**
 * @file set_functions.c
 * Provides several functions to commit and knowns sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdio.h>
#include <set.h>
#include <tools.h>
#include <errors.h>

/**
 * Add a commit to a set
 *
 * @param s Set
 * @param elem Commit
 */
void PICC_set_add_commit(PICC_Set* s, PICC_Commit* elem)
{
    ASSERT(s->set_type == PICC_COMMIT);

    int i;
    PICC_CommitL* current = s->element.commit;

    for(i = 0 ; i<s->size - 1 ; i++)
        current = current->next;

    if(s->size > 0)
    {
        PICC_MALLOC(current->next, PICC_CommitL, NULL);
        current->next->val = elem;
    }
    else
    {
        current->val = elem;
        current->next = NULL;
    }

    s->size++;
}

/**
 * Add a knowns to a set
 *
 * @param s Set
 * @param elem Knowns
 */
void PICC_set_add_knowns(PICC_Set* s, PICC_Knowns* elem)
{
    ASSERT(s->set_type == PICC_KNOWNS)

    printf("ADDING\n");

    int i;
    PICC_KnownsList* current = s->element.knowns;

    for(i = 0 ; i<s->size - 1 ; i++)
        current = current->next;

    if(s->size > 0)
    {
        PICC_MALLOC(current->next, PICC_KnownsList, NULL);
        current->next->val = elem;
    }
    else
    {
        current->val = elem;
        current->next = NULL;
    }

    s->size++;
}

/**
 * Compare 2 commits
 *
 * @param c Commit
 * @param c2 Commit
 * @param err Error
 * @return res true if commits are the same else false
 */
bool PICC_cmp_commit(PICC_Commit* c, PICC_Commit* c2, PICC_Error* err)
{
    bool res = ((c->type == c2->type)
    && (c->thread == c2->thread)
    && (c->clock == c2->clock)
    && (c->clockval == c2->clockval)
    && (c->cont_pc == c2->cont_pc)
    && (c->channel == c2->channel));

    if(c->type == PICC_IN_COMMIT)
        return res && (c->content.in->refvar == c2->content.in->refvar);
    if(c->type == PICC_OUT_COMMIT)
        return res && (c->content.out->eval_func == c2->content.out->eval_func);

    return res;
}

/**
 * Compare 2 knowns
 *
 * @param k Knowns
 * @param k2 Knowns
 * @param err Error
 * @return res true if knowns are the same else false
 */
bool PICC_cmp_knowns(PICC_Knowns* k, PICC_Knowns* k2, PICC_Error* err)
{
    return (k->channel == k2->channel && k->state == k2->state);
}

/**
 * Search if a commit is in a set
 *
 * @param s Set
 * @param c Commit
 * @param err Error
 * @return res true if c is in s
 */
bool PICC_set_mem_commit(PICC_Set* s, PICC_Commit* c, PICC_Error* err)
{
    ASSERT(s->set_type == PICC_COMMIT);

    PICC_CommitL* current = s->element.commit;

    int i;
    for(i = 0 ; i<s->size ; i++)
    {
        if(PICC_cmp_commit(current->val, c, err))
            return true;
        current = current->next;
    }

    return false;
}

/**
 * Search if a knowns is in a set
 *
 * @param s Set
 * @param k knowns
 * @param err Error
 * @return res true if k is in s
 */
bool PICC_set_mem_knowns(PICC_Set* s, PICC_Knowns* k, PICC_Error* err)
{
    ASSERT(s->set_type == PICC_KNOWNS);

    PICC_KnownsList* current = s->element.knowns;

    int i;
    for(i = 0 ; i<s->size ; i++)
    {
        if(PICC_cmp_knowns(current->val, k, err))
            return true;
        current = current->next;
    }

    return false;
}

/**
 * Apply a fonction to all elements of a set
 *
 * @param s Set
 * @param func Function that takes a commit parameter
 */
void PICC_set_map_commit(PICC_Set* s, void (* func)(PICC_Commit*))
{
    int i;
    ASSERT(s->set_type == PICC_COMMIT);

    PICC_CommitL* current = s->element.commit;

    for(i = 0 ; i<s->size ; i++)
    {
        func(current->val);
        current = current->next;
    }
}

/**
 * Apply a fonction to all elements of a set
 *
 * @param s Set
 * @param func Function that takes a knowns parameter
 */
void PICC_set_map_knowns(PICC_Set* s, void (* func)(PICC_Knowns*))
{
    int i;
    ASSERT(s->set_type == PICC_KNOWNS);

    PICC_KnownsList* current = s->element.knowns;

    for(i = 0 ; i<s->size ; i++)
    {
        func(current->val);
        current = current->next;
    }
}
