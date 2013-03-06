/**
 * @file knownset.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#ifndef KNOWNSET_H
#define KNOWNSET_H

#include <stdbool.h>
#include <error.h>

enum
{
    TREE,
    LIST
} KNOWNSET_TYPE;

typedef void* GEN_VALUE;
typedef struct _Tree PICC_Tree;
typedef struct _KnownSet PICC_KnownSet;
typedef struct _KnownSetTree PICC_KnownSetTree;
typedef struct _KnownSetList PICC_KnownSetList;

typedef struct _KnownSetIterator PICC_KnownSetIterator;
typedef struct _KnownSetTreeIterator PICC_KnownSetTreeIterator;
typedef struct _KnownSetListIterator PICC_KnownSetListIterator;


extern PICC_KnownSet *PICC_create_empty_knowns_set();
extern PICC_KnownSet *PICC_create_known_set(int size, PICC_Error* error);

extern bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE elem);
extern int PICC_known_set_size(PICC_KnownSet *s);

//tell if elem is in the set
extern bool PICC_known_set_mem(PICC_KnownSet *s, GEN_VALUE elem);

extern PICC_KnownSetIterator *PICC_create_known_set_iterator(PICC_KnownSet *s);
extern PICC_KnownSetIterator *PICC_delete_known_set_iterator(PICC_KnownSetIterator *it);
extern GEN_VALUEx PICC_known_set_next(PICC_KnownSetIterator *it);
extern bool PICC_known_set_has_next(PICC_KnownSetIterator *it);

#define PICC_KNOWNSET_FOREACH(type, current, set, it)		        \
        do{								\
        type* current;						        \
        PICC_KnownSetIterator it = PICC_create_known_set_iterator(set);	\
        while(PICC_known_set_has_next(it)){				\
	    current = (type*) PICC_known_set_next(it)	 // no ; will be add in the code

#define END_KNOWNSET_FOREACH		        \
        }					\
	    PICC_delete_known_set_iterator(it); \
        }while(0)  // no ; it's intended
#endif
