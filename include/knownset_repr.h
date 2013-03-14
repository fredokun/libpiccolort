/**
 * @file knownset_repr.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#ifndef KNOWNSET_REPR_H
#define KNOWNSET_REPR_H

#include <knownset.h>
#include <error.h>
#include <value_repr.h>

/**
 * The type of a channel with a known state
 */

struct _PICC_Handle
{
    VALUE_HEADER;
    union
    {
        PICC_ChannelValue *chan;
        PICC_StringValue* str;
    } content;
};

struct _PICC_Known
{
    VALUE_HEADER;
    int index_in_known;
    struct _PICC_Handle *handle;
};

// invariants
extern void PICC_Known_inv(PICC_Known *known);

#endif
