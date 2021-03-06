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
 *
 * @inv handle != null
 */
struct _PICC_KnownValue
{
    VALUE_HEADER;
    struct _PICC_Handle *handle; // Defined in gc.h
};

/**
 * Value wrapper used only by the set in his internal behavior
 */
struct _PICC_KnownElement{
    PICC_KnownState state;
    PICC_KnownValue value;
};

#define SET_INIT_MAXSIZE 10

/**
 * A set of known elements.
 *
 * @inv 0 <= current_size <= max_size
 */
struct _PICC_KnownSet
{
    int max_size;
    int current_size;
    struct _PICC_KnownElement *content;
};

extern PICC_KnownElement *PICC_knownset_get_element(PICC_KnownSet *knownset, PICC_KnownValue *val);

// invariants
extern void PICC_KnownSet_inv(PICC_KnownSet *set);
extern void PICC_KnownElement_inv(PICC_KnownElement *elem);
extern void PICC_KnownValue_inv(PICC_KnownValue *val);

//DO NOT DELETE - MACRO USED BY THE GENERATED CODE 
#define PICC_GET_HANDLE(val) ((PICC_KnownValue*) (val))->handle
#define PICC_ACQUIRE_HANDLE(val) LOCK_HANDLE(((PICC_KnownValue*) (val))->handle)
#define PICC_HANDLE_GLOBALRC(val) ((PICC_KnownValue*) (val))->handle->global_rc 


#define PICC_KNOWNSET_FOREACH(s, e)					\
    for(int i = 0; i < (s)->current_size				\
	    && ( (e).header = (s)->content[i].value.header,		\
		 (e).handle = (s)->content[i].value.handle );		\
	i++)


#endif
