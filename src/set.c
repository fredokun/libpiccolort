/**
 * @file set.c
 * Provides several functions to generic sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <set.h>
#include <tools.h>

PICC_Set* PICC_set_make(PICC_SetType type)
{
    //ASSERT((s->set_type == PICC_COMMIT || s->set_type == PICC_KNOWNS))

    ALLOC(s, PICC_Set, NULL);
    s->set_type = type;
    s->size = 0;

    switch(type)
    {
        case PICC_COMMIT:
        ALLOC(s->element.commit, PICC_Commit, NULL);
        break;

        case PICC_KNOWNS:
        ALLOC(s->element.knowns, PICC_Knowns, NULL);
        break;
    }

    return s;
}

void PICC_set_add_commit(PICC_Set* s, PICC_Commit* elem)
{
    //ASSERT(s->set_type != PICC_COMMIT)

    int i;
    PICC_CommitList* current = s->element.commit;

    for(i = 0 ; i<s->size ; i++)
    {
        current = current->next;
    }
    current->val = elem;

    s->size++;

    free(current);
}

void PICC_set_add_knowns(PICC_Set* s, PICC_Knowns* elem)
{
    //ASSERT(s->set_type != PICC_KNOWNS)

    //s->element.knowns = elem;
    //s->size++;
}


