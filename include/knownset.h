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

#define PICC_MAX_LIST 10

enum
{
    TREE,
    LIST
} KNOWNSET_TYPE;

/**
 * The known state of a channel
 */
enum _PICC_KnowsState {
    PICC_UNKNOWN, /**< TODO see spec */
    PICC_KNOWN, /**< TODO see spec */
    PICC_FORGET /**< TODO see spec */
};


typedef struct _PICC_Value GEN_VALUE;
/**
 * The known state of a channel
 */
typedef enum _PICC_KnowsState PICC_KnownsState;
typedef struct _Tree PICC_Tree;
typedef struct _KnownSet PICC_KnownSet;
typedef struct _KnownSetTree PICC_KnownSetTree;
typedef struct _KnownSetList PICC_KnownSetList;

typedef struct _KnownSetIterator PICC_KnownSetIterator;
typedef struct _KnownSetTreeIterator PICC_KnownSetTreeIterator;
typedef struct _KnownSetListIterator PICC_KnownSetListIterator;


extern PICC_KnownSet *PICC_create_empty_known_set();
extern PICC_KnownSet *PICC_create_known_set(int size, PICC_Error* error);

extern void PICC_free_known_set(PICC_KnownSet *s);

extern bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE *elem);
extern int PICC_known_set_size(PICC_KnownSet *s);
extern int PICC_equals(GEN_VALUE *v, GEN_VALUE *v2);

//tell if elem is in the set
extern bool PICC_known_set_mem(PICC_KnownSet *s, GEN_VALUE *elem);

extern PICC_KnownSetIterator *PICC_create_known_set_iterator(PICC_KnownSet *s);
extern PICC_KnownSetIterator *PICC_delete_known_set_iterator(PICC_KnownSetIterator *it);
extern GEN_VALUE *PICC_known_set_next(PICC_KnownSetIterator *it);
extern bool PICC_known_set_has_next(PICC_KnownSetIterator *it);

#define PICC_KNOWNSET_FOREACH(type, current, set, it)		        \
        do{								\
        type* current;						        \
        PICC_KnownSetIterator *it = PICC_create_known_set_iterator(set);	\
        while(PICC_known_set_has_next(it)){				\
	    current = (type*) PICC_known_set_next(it)	 // no ; will be add in the code

#define END_KNOWNSET_FOREACH		        \
        }					\
	    PICC_delete_known_set_iterator(it); \
        }while(0)  // no ; it's intended

extern PICC_KnownSet *PICC_knowns_set_knows(PICC_KnownSet *ks);
extern PICC_KnownSet *PICC_knowns_set_forget(PICC_KnownSet *ks);

extern void PICC_knowns_set_forget_to_unknown_gen(PICC_KnownSet *ks, GEN_VALUE *val);
extern void PICC_knowns_set_forget_all(PICC_KnownSet *ks);

extern bool PICC_knowns_register_gen(PICC_KnownSet *ks, GEN_VALUE *val);
#endif
