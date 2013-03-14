/**
 * @file knownset_repr.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
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

struct _PICC_KnownSet
{
    int max_size;
    int current_size;
    struct _PICC_KnownElement* content;
};
//inv current_size <= max_size

#define PICC_KNOWNSET_FOREACH(s, e)			\
    for(int i = 0;					\
	i < (s)->current_size && ((e) = (s)->content[i].value);	\
	i++)						

// invariants
//extern void PICC_Known_inv(PICC_Known *known);

#endif
