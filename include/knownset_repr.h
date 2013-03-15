/**
 * @file knownset_repr.h
 * Data structures representation for known sets.
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
 * @author Maxence Wo
 * @author Mickaël Menu
 */

#ifndef KNOWNSET_REPR_H
#define KNOWNSET_REPR_H

#include <knownset.h>
#include <error.h>
#include <value_repr.h>
#include <gc_repr.h>

typedef struct _PICC_KnownElement PICC_KnownElement;

/**
 * Abstraction of a managed value for the KnownSet
 * implemented by the PICC_ChannelValue and PICC_StringValue for now
 */
struct _PICC_KnownValue
{
    VALUE_HEADER;
    int index_in_knownset;
    struct _PICC_Handle *handle; // Defined in gc.h
};

/**
 * Value wrapper used only by the set in his internal behavior
 */
struct _PICC_KnownElement{
    PICC_KnownState state;
    PICC_KnownValue* value;
};

#define SET_INIT_MAXSIZE 10

/**
 * inv: current_size <= max_size
 */
struct _PICC_KnownSet
{
    int max_size;
    int current_size;
    struct _PICC_KnownElement* content;
};

#define PICC_KNOWNSET_FOREACH(s, e) \
    for(int i = 0; \
    	i < (s)->current_size && ((e) = (s)->content[i].value);	\
    	i++)

//macro unsafe utilisé dans le code généré
#define PICC_GET_HANDLE(val) ((PICC_KnownValue*) (val))->handle
#define PICC_ACQUIRE_HANDLE(val) LOCK_HANDLE(((PICC_KnownValue*) (val))->handle)
#define PICC_HANDLE_GLOBALRC(val) ((PICC_KnownValue*) (val))->handle->global_rc

/* #define PICC_CHANNEL_KNOWNSET_FOREACH(channel, set, it)              \ */
/*         do{                              \ */
/*         PICC_Channel* channel;                               \ */
/*         PICC_KnownSetIterator *it = PICC_create_knownset_iterator(set);  \ */
/*         while(PICC_known_set_has_next(it)){              \ */
/*      channel = PICC_channel_of_channel_value((PICC_Value*) PICC_known_set_next(it)) */


extern PICC_KnownElement *PICC_knownset_get_element(PICC_KnownSet *knownset, PICC_KnownValue *val);

// invariants
extern void PICC_knownset_inv(PICC_KnownSet *set);

#endif
