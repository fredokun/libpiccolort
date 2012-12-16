/**
 * @file set.h
 * Provides several function headers and structures to generic sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#ifndef SET_H
#define SET_H

#include <channel.h>
#include <commit.h>

typedef enum _PICC_SetType
{
    PICC_COMMIT,
    PICC_KNOWNS
} PICC_SetType;

typedef struct _PICC_CommitList
{
    PICC_Commit* val;
    PICC_Commit* next;
} PICC_CommitList;

typedef struct _PICC_KnownsList
{
    PICC_Knowns* val;
    PICC_Knowns* next;
} PICC_KnownsList;

typedef struct _PICC_Set
{
    PICC_SetType set_type;
    int size;

    union
    {
        PICC_CommitList* commit;
        PICC_KnownsList* knowns;
    } element;
} PICC_Set;

PICC_Set* PICC_set_make(PICC_SetType type);
void PICC_set_add_commit(PICC_Set* s, PICC_Commit* elem);
void PICC_set_add_knowns(PICC_Set* s, PICC_Knowns* elem);

#define PICC_SET_ADD(s,e) \
    switch(s->set_type) \
    { \
        case PICC_COMMIT: \
        PICC_set_add_commit(s,e); \
        break; \
        \
        case PICC_KNOWNS: \
        PICC_set_add_knowns(s,e); \
        break; \
    }

#endif
