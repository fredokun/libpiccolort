/**
 * @file knownset.c
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
 * @author Maxence Wo
 * @author Mickaël Menu
 */

#include <stdio.h>
#include <stdlib.h>

#include <knownset_repr.h>
#include <tools.h>
#include <errors.h>
#include <value_repr.h>

#define PICC_KNOWNSET_FOREACH_ELEM(s, e) \
    for(int i = 0; \
    i < (s)->current_size && (e = &((s)->content[i])); \
    i++)

// Life cycle //////////////////////////////////////////////////////////////////

/**
 * Creates a new known set with given initial size.
 *
 * @pre init_max_size > 0
 * @post knownset.max_size = init_max_size
 * @post knowset.current_size = 0
 * @post forall (elem in knownset) { elem.state = UNKNOWN and elem.value = null }
 * @param init_max_size Initial set size
 * @param error Error stack
 * @return Created known set
 */
PICC_KnownSet *PICC_create_knownset(int init_max_size, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        // pre: init_max_size > 0
        ASSERT(init_max_size > 0);
    #endif

    PICC_ALLOC(knownset, PICC_KnownSet, error) {
        knownset->max_size = init_max_size;
        knownset->current_size = 0;

        PICC_KnownElement *elts = malloc(sizeof(PICC_KnownElement) * init_max_size);
        if (elts == NULL) {
            NEW_ERROR(error, ERR_OUT_OF_MEMORY);
        } else {
            for (int i = 0; i < init_max_size; i++) {
                elts[i].state = PICC_UNKNOWN;
                elts[i].value = NULL;
            }
            knownset->content = elts;
        }
    }

    #ifdef CONTRACT_POST
        // post: knownset.max_size = init_max_size
        ASSERT(knownset->max_size == init_max_size);
        // post: knowset.current_size = 0
        ASSERT(knownset->current_size == 0);
        // post: forall (elem in knownset) { elem.state = UNKNOWN and elem.value = null }
        PICC_KnownElement *elem;
        for (int i = 0; i < knownset->max_size; i++) {
            elem = &(knownset->content[i]);
            ASSERT(elem->state == PICC_UNKNOWN);
            ASSERT(elem->value == NULL);
        }
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(knownset);
    #endif

    return knownset;
}

/**
 * Creates a new empty known set.
 *
 * @return Empty known set
 */
PICC_KnownSet *PICC_create_empty_knownset()
{
    ALLOC_ERROR(error);
    PICC_KnownSet *set = PICC_create_knownset(SET_INIT_MAXSIZE, &error);
    if (HAS_ERROR(error))
        CRASH(&error);

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(set);
    #endif

    return set;
}

/**
 * Deallocates the given known set.
 *
 * @param knownset Known set to deallocate
 */
void PICC_free_knownset(PICC_KnownSet *knownset)
{
    if (knownset != NULL) {
        free(knownset->content);
        free(knownset);
    }
}

/**
 * Gets the corresponding known element to the given known value in the given
 * known set. Returns NULL if the known value is not inside the known set.
 *
 * @pre knownset != null
 * @pre val != null
 * @post if (val in knownset) then elem.value = val
 * @post if (val not in knownset) then elem = null
 * @param knownset Known set
 * @param val Known value
 * @return The known element corresponding to the known value
 */
PICC_KnownElement *PICC_knownset_get_element(PICC_KnownSet *knownset, PICC_KnownValue *val)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(knownset);
        PICC_KnownValue_inv(val);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(knownset != NULL);
        ASSERT(val != NULL);
    #endif

    PICC_KnownElement *elem;
    bool found = false;
    PICC_KNOWNSET_FOREACH_ELEM(knownset, elem) {
        if (elem->value == val) {
            found = true;
            break;
        }
    }

    if (!found)
        elem = NULL;

    #ifdef CONTRACT_POST
        if (found) {
            ASSERT(elem->value == val);
        } else {
            ASSERT(elem == NULL);
        }
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(knownset);
        PICC_KnownValue_inv(val);
    #endif

    return elem;
}

/**
 * Looks for the given channel in the given known set :
 *   if the channel is in the known set :
 *     if the channel is in the KNOWN state, returns false
 *     if the channel is in the FORGET state, switches it to KNOWN and returns false
 *   else adds the channel to the known set with a KNOWN state and returns true
 *
 * @pre ks != null
 * @pre val != null
 * @post if (val in ks and val@pre.state = KNOWN) then false
 * @post if (val in ks and val@pre.state = FORGET) then (val.state = KNOWN), false
 * @post if (val not in ks) then val in ks, val.state = KNOWN, true
 * @param ks Known set
 * @param val Channel value
 * @return Whether the channel has been added to the known set
 */
bool PICC_knownset_register(PICC_KnownSet *ks, PICC_KnownValue *val)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
        ASSERT(val != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        PICC_KnownElement *val_at_pre = PICC_knownset_get_element(ks, val);
        bool val_in_ks_at_pre = (val_at_pre != NULL);
    #endif

    bool channel_added_to_knownset = false;

    PICC_KnownElement *elem = PICC_knownset_get_element(ks, val);
    if (elem != NULL) { // already in set
        if (elem->state == PICC_FORGET)
            elem->state = PICC_KNOWN;

    } else {
        channel_added_to_knownset = true;
        PICC_knownset_add(ks, val);
        elem = PICC_knownset_get_element(ks, val);
        elem->state = PICC_KNOWN;
    }

    #ifdef CONTRACT_POST
        if (val_in_ks_at_pre) {
            // post: if (val in ks and val@pre.state = KNOWN) then false
            // post: if (val in ks and val@pre.state = FORGET) then (val.state = KNOWN), false
            if (val_at_pre->state == PICC_FORGET)
                ASSERT(PICC_knownset_get_element(ks, val)->state == PICC_KNOWN);
            ASSERT(channel_added_to_knownset == false);

        // post: if (val not in ks) then val in ks, val.state = KNOWN, true
        } else {
            PICC_KnownElement *elem = PICC_knownset_get_element(ks, val);
            ASSERT(elem != NULL);
            ASSERT(elem->state == PICC_KNOWN);
            ASSERT(channel_added_to_knownset == true);
        }
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif

    return channel_added_to_knownset;
}

/**
 * Adds the given known value to the given known set.
 *
 * @pre ks != null
 * @pre val != null
 * @post val in ks
 * @param ks Known set
 * @param val Known value
 * @return Whether the value has been added in the set
 */
bool PICC_knownset_add(PICC_KnownSet *ks, PICC_KnownValue *val)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
        ASSERT(val != NULL);
    #endif

    PICC_KnownElement *elem;

    bool new_add = false;
    elem = PICC_knownset_get_element(ks, val);
    if (elem == NULL) { // not in set, we need to add it
        if (ks->current_size == ks->max_size) { // set full, we need to realloc
        	int new_max_size = ks->current_size + SET_INIT_MAXSIZE;
        	PICC_KnownElement *new_elts = realloc(ks->content, sizeof(PICC_KnownElement) * new_max_size);
        	if (new_elts == NULL) {
                CRASH_NEW_ERROR(ERR_OUT_OF_MEMORY);
            }

        	for (int i = ks->current_size; i < ks->max_size; i++) {
                new_elts[i].state = PICC_UNKNOWN;
                new_elts[i].value = NULL;
        	}

            ks->max_size = new_max_size;
        	ks->content = new_elts;
        }

        ks->content[ks->current_size].value = val;
        ks->current_size ++;
        new_add= true;
    }

    #ifdef CONTRACT_POST
        ASSERT(PICC_knownset_get_element(ks, val) != NULL);
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif

    return new_add;
}

/**
 * Returns the size of the given known set.
 *
 * @param knownset Known set
 * @return Size of the known set
 */
int PICC_knownset_size(PICC_KnownSet *knownset)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(knownset);
    #endif

    return knownset->current_size;
}


// Subsets creation ////////////////////////////////////////////////////////////

/**
 * Returns a subset of all KNOWN-state in the given known set.
 *
 * @pre ks != null
 * @post forall (elem in ks) {
 *   if (elem.state = KNOWN) then
 *     elem in subset
 *   else
 *     elem not in subset
 *   }
 * @param ks Known set
 * @return Subset of all KNOWN-state
 */
PICC_KnownSet *PICC_knownset_known(PICC_KnownSet *ks)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
    #endif

    PICC_KnownSet* known_subset = PICC_create_empty_knownset();

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
    	if (elem->state == PICC_KNOWN)
    	    PICC_knownset_add(known_subset, elem->value);
    }

    #ifdef CONTRACT_POST
        // ...
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
    #endif

    return known_subset;
}

/**
 * Returns a subset of all FORGET-state in the given known set.
 *
 * @pre ks != null
 * @post forall (elem in ks) {
 *   if (elem.state = FORGET) then
 *     elem in subset
 *   else
 *     elem not in subset
 *   }
 * @param ks Known set
 * @return Subset of all FORGET-state
 */
PICC_KnownSet *PICC_knownset_forget(PICC_KnownSet *ks)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
    #endif

    PICC_KnownSet* forget_subset = PICC_create_empty_knownset();

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
    	if(elem->state == PICC_FORGET)
    	    PICC_knownset_add(forget_subset, elem->value);
    }

    #ifdef CONTRACT_POST
        // ...
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
    #endif

    return forget_subset;
}


// State changes ///////////////////////////////////////////////////////////////

/**
 * Switches the given known value of the known set from the FORGET state to the
 * UNKNOWN state.
 *
 * @pre ks != NULL
 * @pre val != NULL
 * @post if (val in ks and val@pre.state = FORGET) then val.state = UNKNOWN
 * @param ks Known state
 * @param val Known value
 */
void PICC_knownset_forget_to_unknown(PICC_KnownSet *ks, PICC_KnownValue *val)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
    #endif

    #ifdef CONTRACT_POST
        // captures
        PICC_KnownElement *elem1 = PICC_knownset_get_element(ks, val);
        bool val_in_ks_at_pre = (elem1 != NULL);
        PICC_KnownState state_at_pre;
        if (val_in_ks_at_pre)
            state_at_pre = elem1->state;
    #endif

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
        if (elem->value == val && elem->state == PICC_FORGET)
            elem->state = PICC_UNKNOWN;
    }

    #ifdef CONTRACT_POST
        PICC_KnownElement *elem2 = PICC_knownset_get_element(ks, val);
        if (val_in_ks_at_pre && state_at_pre == PICC_FORGET) {
            ASSERT(elem2->state == PICC_UNKNOWN);
        }
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
        PICC_KnownValue_inv(val);
    #endif
}

/**
 * Switches all KNOWN-state element of the given known set to the FORGET state.
 *
 * @pre ks != null
 * @post forall (elem in ks) { if (elem@pre.state = KNOWN) then elem.state = FORGET }
 * @param ks Known set
 */
void PICC_knownset_forget_all(PICC_KnownSet *ks)
{
    #ifdef CONTRACT_PRE_INV
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        ASSERT(ks != NULL);
    #endif

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
        if (elem->state == PICC_KNOWN)
        	elem->state = PICC_FORGET;
    }

    #ifdef CONTRACT_POST
        // post: forall (elem in ks) { if (elem@pre.state = KNOWN) then elem.state = FORGET }
        // ...
    #endif

    #ifdef CONTRACT_POST_INV
        PICC_KnownSet_inv(ks);
    #endif
}


// Invariants //////////////////////////////////////////////////////////////////

void PICC_KnownSet_inv(PICC_KnownSet *set)
{
    if (set) {
        // inv: 0 <= current_size <= max_size
        ASSERT(0 <= set->current_size);
        ASSERT(set->current_size <= set->max_size)
    }
}

void PICC_KnownElement_inv(PICC_KnownElement *elem)
{
}

void PICC_KnownValue_inv(PICC_KnownValue *val)
{
    if (val) {
        // inv: handle != null
        ASSERT(val->handle != NULL);
    }
}
