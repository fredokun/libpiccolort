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
    PICC_KNOWNS,
} PICC_SetType;

typedef struct _PICC_CommitL PICC_CommitL;
struct _PICC_CommitL
{
    PICC_Commit* val;
    PICC_CommitL* next;
};

typedef struct _PICC_KnownsList PICC_KnownsList;
struct _PICC_KnownsList
{
    PICC_Knowns* val;
    PICC_KnownsList* next;
};

typedef struct _PICC_Set
{
    PICC_SetType set_type;
    int size;

    union
    {
        PICC_CommitL* commit;
        PICC_KnownsList* knowns;
    } element;
} PICC_Set;

typedef struct _PICC_SetElement
{
    PICC_SetType type;

    union
    {
        PICC_Commit* c;
        PICC_Knowns* k;
    } element;
} PICC_SetElement;

extern bool PICC_cmp_commit(PICC_Commit* c, PICC_Commit* c2, PICC_Error* err);
extern bool PICC_cmp_knowns(PICC_Knowns* k, PICC_Knowns* k2, PICC_Error* err);
extern bool PICC_set_mem_knowns(PICC_Set* s, PICC_Knowns* k, PICC_Error* err);
extern bool PICC_set_mem_commit(PICC_Set* s, PICC_Commit* c, PICC_Error* err);
extern void PICC_set_add_commit(PICC_Set* s, PICC_Commit* elem);
extern void PICC_set_add_knowns(PICC_Set* s, PICC_Knowns* elem);
extern void PICC_set_map_commit(PICC_Set* s, void (* func)(PICC_Commit*));
extern void PICC_set_map_knowns(PICC_Set* s, void (* func)(PICC_Knowns*));

extern PICC_Set* PICC_set_make(PICC_SetType type);
extern void PICC_set_add(PICC_Set* s, PICC_SetElement* elem, PICC_Error* err);
extern bool PICC_set_mem(PICC_Set* s, PICC_SetElement* elem, PICC_Error* err);
extern bool PICC_set_is_empty(PICC_Set* s);
extern void PICC_set_map(PICC_Set* s, void (* func)(void*));


#define PICC_SET_ADD(s,e) \
    PICC_set_add(s, e, NULL)

#define PICC_SET_IS_EMPTY(s) \
    PICC_set_is_empty(s)

#define PICC_SET_MEM(s,e) \
    PICC_set_mem(s, e)

#define PICC_SET_MAP(s,f) \
    PICC_set_map(s, f)

#endif
