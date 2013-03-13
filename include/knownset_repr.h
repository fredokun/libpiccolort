/**
 * @file knownset_repr.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Loïc GIRAULT
 */

#ifndef KNOWNSET_REPR_H
#define KNOWNSET_REPR_H

#include <knownset.h>
#include <error.h>

/**
 * The type of a value with a known state
 */
struct _PICC_Knowns {
    /**@{*/
    GEN_VALUE *val; /**< a reference to the tracked channel */
    PICC_KnownState state; /**< the known state */
    /**@}*/
};

/**
 * The type of a channel with a known state
 */
typedef struct _PICC_Knowns PICC_Knowns;

/**
*   Knownset data structures
*/
struct _Tree
{
    PICC_Tree *father;
    PICC_Tree *left;
    PICC_Tree *right;
    PICC_Knowns known_val;
};

struct _KnownSetTree
{
    int type;
    PICC_Tree *tree;
};

struct _KnownSetList
{
    int type;
    int size;
    struct _PICC_Knowns* liste;
};

struct _KnownSet
{
    int type;
};

struct _KnownSetIterator
{
    PICC_KnownSet *set;
};

struct _KnownSetTreeIterator
{
    PICC_KnownSetTree *set;
    PICC_Tree *previous;
    PICC_Tree *current;
    PICC_Tree *next;
};

struct _KnownSetListIterator
{
    PICC_KnownSetList *set;
    int next;
};

// tree structure
extern void PICC_free_tree(PICC_Tree *tree);
extern void PICC_free_known_set_tree(PICC_KnownSetTree *s);

extern PICC_KnownSetTreeIterator *PICC_create_known_set_tree_iterator(PICC_KnownSetTree *s);
extern PICC_KnownSetTreeIterator *PICC_delete_known_set_tree_iterator(PICC_KnownSetTreeIterator *it);
extern GEN_VALUE *PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it, bool check);
extern PICC_KnownState PICC_known_set_tree_iterator_state(PICC_KnownSetTreeIterator *it);
extern void PICC_known_set_tree_iterator_state_set(PICC_KnownSetTreeIterator *it, PICC_KnownState state);
extern bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetTreeIterator *it);

// list structure
extern void PICC_free_known_set_list(PICC_KnownSetList *s);

extern PICC_KnownSetListIterator *PICC_create_known_set_list_iterator(PICC_KnownSetList *s);
extern PICC_KnownSetListIterator *PICC_delete_known_set_list_iterator(PICC_KnownSetListIterator *it);
extern GEN_VALUE *PICC_known_set_list_iterator_next(PICC_KnownSetListIterator *it);
extern PICC_KnownState PICC_known_set_list_iterator_state(PICC_KnownSetListIterator *it);
extern void PICC_known_set_list_iterator_state_set(PICC_KnownSetListIterator *it, PICC_KnownState state);
extern bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator *it);

// knowns
extern PICC_KnownState PICC_known_set_iterator_state(PICC_KnownSetIterator *it);
extern void PICC_known_set_iterator_state_set(PICC_KnownSetIterator *it, PICC_KnownState state);
extern bool PICC_known_set_add_with_state(PICC_KnownSet *s, GEN_VALUE *elem, PICC_KnownState state);
extern PICC_Knowns *PICC_create_knowns(GEN_VALUE *val, PICC_Error *error);

#define PICC_KNOWNSET_STATE_FOREACH(type, current_val, current_state, set, it)		        \
        PICC_KNOWNSET_FOREACH(type, current_val, set, it);	 \
        PICC_KnownState current_state = PICC_known_set_iterator_state(it)
        
#define PICC_KNOWNSET_ONLY_STATE_FOREACH(type, state, set, it)		        \
        do{								\
        PICC_KnownSetIterator *it = PICC_create_known_set_iterator(set);	\
        while(PICC_known_set_has_next(it)){				\
	    PICC_known_set_next(it); \
        PICC_KnownState state = PICC_known_set_iterator_state(it)

// invariants
extern void PICC_Knowns_inv(PICC_Knowns *knowns);
extern void PICC_KnownSet_inv(PICC_KnownSet *ks);

#endif
