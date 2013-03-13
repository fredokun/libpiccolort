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

/**
 * The type of a channel with a known state
 */

struct _PiResource
{
    int type;
    PICC_KnownState state;

    union
    {
        PICC_ChannelValue* chan;
        PICC_StringValue* str;
    } content;
};

struct _PICC_KnownSet
{
    int size;
    struct _PiResource* resource;
};


// knowns
extern PICC_PiResource *PICC_create_knowns(GEN_VALUE *val, PICC_Error *error);

// invariants
extern void PICC_PiResource_inv(PICC_PiResource *knowns);
extern void PICC_KnownSet_inv(PICC_KnownSet *ks);

#endif
