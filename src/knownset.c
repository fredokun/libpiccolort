/**
 * @file knownset.c
 * Provides several functions to commit and knowns sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdio.h>
#include <stdlib.h>

#include <knownset_repr.h>
#include <tools.h>
#include <errors.h>
#include <value_repr.h>

/**
 * Create an empty known_set
 *
 * @return a newly created known
 */
PICC_Known* PICC_create_empty_known(VALUE_HEADER)
{
    return PICC_create_known(0, header);
}

/**
 * Create a known.
 *
 * @pre size >= 0
 * @pre header == (TAG_CHANNEL || TAG_STRING)
 *
 * @post s != NULL
 * @post s->header == (TAG_CHANNEL || TAG_STRING)
 * @post s->index_in_known == NOT_INDEXED
 * @post s->handle != NULL
 *
 * @return created known
 */
PICC_Known* PICC_create_known(int size, VALUE_HEADER)
{

#ifdef CONTRACT_PRE
    ASSERT(size >= 0);
    ASSERT(header == TAG_CHANNEL || header == TAG_STRING);
#endif

    PICC_Known* s = malloc(sizeof(PICC_Known));
    s->header = MAKE_HEADER(header, NULL);
    s->index_of_known = NOT_INDEXED;
    s->handle = malloc(sizeof(PICC_Handle));

    if(IS_CHANNEL(s->header))
    {
        s->handle->header = TAG_CHANNEL;
        s->handle->content.chan = malloc(size * sizeof(PICC_ChannelValue));
    }
    if(IS_STRING(s->header))
    {
        s->handle->header = TAG_STRING;
        s->handle->content.chan = malloc(size * sizeof(PICC_StringValue));
    }

#ifdef CONTRACT_POST_INV
    PICC_Known_inv(s);
#endif

#ifdef CONTRACT_POST
    ASSERT(s != NULL);
    ASSERT(s->header == TAG_STRING || s->header == TAG_CHANNEL);
    ASSERT(s->index_in_known == NOT_INDEXED);
    ASSERT(s->handle != NULL);
#endif

    return s;
}

/**
 * Free a known.
 *
 * @pre s != NULL
 *
 */
void PICC_free_known(PICC_Known *s) 
{
#ifdef CONTRACT_PRE
    ASSERT(s != NULL);
#endif

    if(s->type == TREE)
        PICC_free_known_tree((PICC_KnownTree *)s);
    else
        PICC_free_known_list((PICC_KnownList *)s);
}

/**
 * Add an element to a set
 *
 * @param s known
 * @param elem the value to add to the known s
 */
 bool PICC_known_add(PICC_Known *s, GEN_VALUE *elem)
{
    return PICC_known_add_with_state(s, elem, PICC_KNOWN);    
}

/**
 * Calculate the size of a known
 *
 * @param s PICC_Known
 *
 * @return size of the known
 */
int PICC_known_size(PICC_Known *s)
{
    if(s->type == TREE)
        return PICC_known_size_tree(s);
    else
        return PICC_known_size_list(s);
}


/**
 * Returns a subset of all KNOWN-STATE in a knows set.
 *
 * @param ks Knowns set
 * @return Subset of all known channel in the given set
 */
PICC_Known *PICC_known_set_knows(PICC_Known *ks)
{
    return PICC_knowns_set_search(ks, PICC_KNOWN);
}

/**
 * Returns a subset of all FORGET-STATE in a knowns set.
 *
 * @param ks Knowns set
 * @return Subset of all forget state in the given set.
 */
PICC_Known *PICC_known_set_forget(PICC_Known *ks)
{
    return PICC_knowns_set_search(ks, PICC_FORGET);
}

/**
 * Switches an element of a KnowsSet from the FORGET state to the
 * UNKNOWN state.
 *
 * @pre ks != NULL
 * @pre ch != NULL

 * @param ks Knows set
 * @param ch Channel to switch state
 */
void PICC_known_set_forget_to_unknown_gen(PICC_Known *ks, GEN_VALUE *val)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_Known_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
        ASSERT(val != NULL);
    #endif

    PICC_KNOWN_FOREACH(GEN_VALUE, value, ks, it);
        if(compare_values(val, value) == 0){
            PICC_known_iterator_state_set(it, PICC_UNKNOWN);
        }
    END_KNOWN_FOREACH;

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_Known_inv(ks);
    #endif
}

/**
 * Switches all KNOWN state elements of a KnowsSet to FORGET state.
 *
 * @pre ks != NULL
 *
 * @post ks only have knowns values with PICC_FORGET state
 *
 * @param ks Knows set
 */
void PICC_known_set_forget_all(PICC_Known *ks)
{
    #ifdef CONTRACT_PRE_INV
         //inv
         PICC_Known_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
         //pre
         ASSERT(ks != NULL);
    #endif

    PICC_KNOWN_ONLY_STATE_FOREACH(GEN_VALUE, state, ks, it);
        if (state == PICC_KNOWN) {
            PICC_known_iterator_state_set(it, PICC_FORGET);
        }
    END_KNOWN_FOREACH;

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_Known_inv(ks);
    #endif

    #ifdef CONTRACT_POST
        // post
        PICC_KNOWN_ONLY_STATE_FOREACH(GEN_VALUE, state, ks, it);
            ASSERT(state != PICC_KNOWN );
        END_KNOWN_FOREACH;
    #endif
}

/**
 * Adds a channel to a knowns set.
 *
 * Looks for a channel in a PICC_Known
 * - if the channel is in the PICC_Known in KNOWN-STATE, it returns false
 * - if the channel is in the PICC_Known in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PICC_Known (KNOWS-STATE) then returns true
 *
 * @pre ks != NULL
 *
 * @param ks Knowns set
 * @param ch Channel to add
 * @return Whether the channel has been added
 */
bool PICC_known_register_gen(PICC_Known *ks, GEN_VALUE *val)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_Known_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
    #endif

    bool registered = true;
    GEN_VALUE* val0 = NULL;

    PICC_KNOWN_STATE_FOREACH(GEN_VALUE, value, state, ks, it);
        if(compare_values(val, value) == 0)
        {
            val0 = value;
            if(state == PICC_KNOWN || state == PICC_FORGET)
            {
                registered = false;
            }
            if(state == PICC_UNKNOWN || state == PICC_FORGET)
            {
                PICC_known_iterator_state_set(it, PICC_KNOWN);
            }
            break;
        }
    END_KNOWN_FOREACH;

    if (val0 == NULL)
    {
        registered = PICC_known_add(ks, val);
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_Known_inv(ks);
    #endif

    return registered;
}

// Invariants ///////////////////////////////////////

/**
 * Checks knownsSet invariant.
 *
 * @inv knownsSet->size > -1
 */
void PICC_Known_inv(PICC_Known *set)
{
    ASSERT(PICC_known_size(set) > -1);
}
