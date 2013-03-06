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

/**
*   Knownset data structures
*/
struct _Tree
{
    PICC_Tree *father;
    PICC_Tree *left;
    PICC_Tree *right;
    GEN_VALUE val;
};

struct _KnownsetTree
{
    int type;
    PICC_Tree *tree;
};

struct _KnownsetList
{
    int type;
    int size;
    GEN_VALUE* liste;
};

struct _Knownset
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
    PICC_Tree *current;
    PICC_Tree *next;
};

struct _KnownSetListIterator
{
    PICC_KnownSetList *set;
    int next;
};

// tree structure
extern PICC_KnownSetTreeIterator *PICC_create_known_set_tree_iterator(PICC_KnownSetTree *s);
extern PICC_KnownSetTreeIterator *PICC_delete_known_set_tree_iterator(PICC_KnownSetTreeIterator *it);
extern GEN_VALUE PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it);
extern GEN_VALUE PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it);
extern bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetListIterator *it);

// list structure
extern PICC_KnownSetListIterator *PICC_create_known_set_list_iterator(PICC_KnownSetList *s);
extern PICC_KnownSetListIterator *PICC_delete_known_set_list_iterator(PICC_KnownSetListIterator *it);
extern GEN_VALUE PICC_known_set_list_iterator_next(PICC_KnownSetListIterator *it);
extern bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator *it);

// invariants
extern void PICC_KnownSet_inv(PICC_KnownSet *ks);

#endif
