#include <stdio.h>
#include <set.h>
#include <tools.h>
#include <errors.h>

PICC_Set* PICC_set_make(PICC_SetType type)
{
    ASSERT((type == PICC_COMMIT || type == PICC_KNOWNS))

    PICC_ALLOC(s, PICC_Set, NULL)
    {
        s->set_type = type;
        s->size = 0;
    }

    if(s->set_type == PICC_COMMIT)
    {
        PICC_MALLOC((s->element.commit), PICC_CommitL, NULL);
        PICC_MALLOC(s->element.commit->val, PICC_Commit, NULL);
        PICC_MALLOC(s->element.commit->next, PICC_CommitL, NULL);
    }
    if(s->set_type == PICC_KNOWNS)
    {
        PICC_MALLOC(s->element.knowns, PICC_KnownsList, NULL);
        PICC_MALLOC(s->element.knowns->val, PICC_Knowns, NULL);
        PICC_MALLOC(s->element.knowns->next, PICC_KnownsList, NULL);
    }

    return s;
}

void PICC_set_add(PICC_Set* s, PICC_SetElement* elem, PICC_Error* err)
{
    switch(s->set_type)
    {
        case PICC_COMMIT:
        PICC_set_add_commit(s,elem->element.c);
        break;

        case PICC_KNOWNS:
        PICC_set_add_knowns(s,elem->element.k);
        break;

        default:
            NEW_ERROR(err, ERR_INVALID_TYPE);
    }
}

bool PICC_set_mem(PICC_Set* s, PICC_SetElement* elem, PICC_Error* err)
{
    switch(s->set_type)
    {
        case PICC_COMMIT:
        return PICC_set_mem_commit(s,elem->element.c, err);

        case PICC_KNOWNS:
        return PICC_set_mem_knowns(s,elem->element.k, err);

        default:
            NEW_ERROR(err, ERR_INVALID_TYPE);
            return false;
    }
}

bool PICC_set_is_empty(PICC_Set* s)
{
    return (s->size == 0);
}

void PICC_set_map(PICC_Set* s, void (*func)(void*))
{
    if(s->set_type == PICC_COMMIT)
    {
        PICC_set_map_commit(s, (void*)(PICC_Commit*)func);
    }
    if(s->set_type == PICC_KNOWNS)
    {
        PICC_set_map_knowns(s, (void*)(PICC_Knowns*)func);
    }
}

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
