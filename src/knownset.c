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
 * @param init_max_size Initial set size
 * @param error Error stack
 * @return Created known set
 */
PICC_KnownSet *PICC_create_knownset(int init_max_size, PICC_Error* error)
{
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

    return knownset;
}

/**
 * Creates a new empty known set.
 *
 * @return Empty known set
 */
PICC_KnownSet *PICC_create_empty_knownset()
{
    return PICC_create_knownset(SET_INIT_MAXSIZE, NULL);
}

/**
 * Deallocates the given known set.
 *
 * @param knownset Known set to deallocate
 */
void PICC_free_knownset(PICC_KnownSet *knownset)
{
    free(knownset->content);
    free(knownset);
}

/**
 * Gets the corresponding known element to the given known value in the given
 * known set. Returns NULL if the known value is not inside the known set.
 *
 * @param knownset Known set
 * @param val Known value
 * @return The known element corresponding to the known value
 */
PICC_KnownElement *PICC_knownset_get_element(PICC_KnownSet *knownset, PICC_KnownValue *val)
{
    PICC_KnownElement *elem = NULL;

    if (val->index_in_knownset > -1) {
        elem = knownset->content + val->index_in_knownset;
        if (elem->value != val) {
            CRASH_NEW_ERROR(ERR_INVALID_KNOWNSET_STATE);
        }
    }

    return elem;
}

/**
 * Looks for the given channel in the given known set :
 *   if the channel is in the known set :
 *     if the channel is in the KNOWN state, returns false
 *     if the channel is in the FORGET state, switches it to KNOWN and returns false
 *   else adds the channel to the known set with a KNOWN state and returns true
 *
 * @param ks Known set
 * @param val Channel value
 * @return Whether the channel has been added to the known set
 */
bool PICC_knownset_register(PICC_KnownSet *ks, PICC_KnownValue *val)
{
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

    return channel_added_to_knownset;
}

/**
 * Adds the given known value to the given known set.
 *
 * @param ks Known set
 * @param val Known value
 */
void PICC_knownset_add(PICC_KnownSet *ks, PICC_KnownValue *val)
{
    PICC_KnownElement *elem;

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
        val->index_in_knownset = ks->current_size;
        ks->current_size ++;
    }
}

/**
 * Returns the size of the given known set.
 *
 * @param knownset Known set
 * @return Size of the known set
 */
int PICC_known_size(PICC_KnownSet *knownset)
{
    return knownset->current_size;
}


// Subsets creation ////////////////////////////////////////////////////////////

/**
 * Returns a subset of all KNOWN-state in the given known set.
 *
 * @param ks Known set
 * @return Subset of all KNOWN-state
 */
PICC_KnownSet *PICC_knownset_known(PICC_KnownSet *ks)
{
    PICC_KnownSet* known_subset = PICC_create_empty_knownset();

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
    	if (elem->state == PICC_KNOWN)
    	    PICC_knownset_add(known_subset, elem->value);
    }

    return known_subset;
}

/**
 * Returns a subset of all FORGET-state in the given known set.
 *
 * @param ks Known set
 * @return Subset of all FORGET-state
 */
PICC_KnownSet *PICC_knownset_forget(PICC_KnownSet *ks)
{
    PICC_KnownSet* forget_subset = PICC_create_empty_knownset();

    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
    	if(elem->state == PICC_FORGET)
    	    PICC_knownset_add(forget_subset, elem->value);
    }

    return forget_subset;
}


// State changes ///////////////////////////////////////////////////////////////

/**
 * Switches the given known value of the known set from the FORGET state to the
 * UNKNOWN state.
 *
 * @param ks Known state
 * @param val Known value
 */
void PICC_knownset_forget_to_unknown(PICC_KnownSet *ks, PICC_KnownValue *val)
{
    ks->content[val->index_in_knownset].state = PICC_FORGET;
}

/**
 * Switches all KNOWN-state element of the given known set to the FORGET state.
 *
 * @param ks Known set
 */
void PICC_knownset_forget_all(PICC_KnownSet *ks)
{
    PICC_KnownElement *elem;
    PICC_KNOWNSET_FOREACH_ELEM(ks, elem) {
    	elem->state = PICC_FORGET;
    }
}


// Invariants //////////////////////////////////////////////////////////////////

void PICC_knownset_inv(PICC_KnownSet *set)
{
    ASSERT(set->current_size<= set->max_size)
}
