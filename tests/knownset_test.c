/**
 * @file knownset_test.c
 * Unit testing of known set
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <value_repr.h>
#include <channel_repr.h>
#include <knownset_repr.h>
#include <error.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))


void test_knownset_creation(PICC_Error *error)
{
    PICC_KnownSet *ks;

    ks = PICC_create_empty_knownset();
    PICC_knownset_inv(ks);

    ks = PICC_create_knownset(5, error);
    ASSERT_NO_ERROR();
    PICC_knownset_inv(ks);
    ASSERT(ks->max_size == 5 && ks->current_size == 0);
}

void test_register(PICC_Error *error)
{
    PICC_KnownSet *ks = PICC_create_empty_knownset();
    PICC_KnownValue *val = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_KnownElement *elem;

    ASSERT(PICC_knownset_register(ks, val) == true);
    elem = PICC_knownset_get_element(ks, val);
    ASSERT(elem->state == PICC_KNOWN);
    ASSERT(PICC_knownset_register(ks, val) == false);
    ASSERT(elem->state == PICC_KNOWN);

    elem->state = PICC_FORGET;
    ASSERT(PICC_knownset_register(ks, val) == false);
    ASSERT(elem->state == PICC_KNOWN);

    elem->state = PICC_UNKNOWN;
    ASSERT(PICC_knownset_register(ks, val) == false);
    ASSERT(elem->state == PICC_UNKNOWN);
}

void test_add(PICC_Error *error)
{
    PICC_KnownSet *ks = PICC_create_empty_knownset();
    PICC_KnownValue *val = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());

    ASSERT(PICC_knownset_size(ks) == 0);
    PICC_knownset_add(ks, val);
    ASSERT(PICC_knownset_size(ks) == 1);
    PICC_knownset_add(ks, val);
    ASSERT(PICC_knownset_size(ks) == 1);
    val = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_knownset_add(ks, val);
    ASSERT(PICC_knownset_size(ks) == 2);
}

void test_known_subset(PICC_Error *error)
{
    PICC_KnownSet *ks = PICC_create_empty_knownset();
    PICC_KnownValue *val_forget = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_KnownValue *val_unknown = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_KnownValue *val_known1 = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_KnownValue *val_known2 = (PICC_KnownValue *)PICC_create_channel_value(PICC_create_channel());
    PICC_KnownElement *elem;

    PICC_knownset_add(ks, val_forget);
    PICC_knownset_add(ks, val_unknown);
    PICC_knownset_add(ks, val_known1);
    PICC_knownset_add(ks, val_known2);

    ASSERT(PICC_knownset_size(ks) == 4);
    elem = PICC_knownset_get_element(ks, val_forget);
    elem->state = PICC_FORGET;
    elem = PICC_knownset_get_element(ks, val_unknown);
    elem->state = PICC_UNKNOWN;
    elem = PICC_knownset_get_element(ks, val_known1);
    elem->state = PICC_KNOWN;
    elem = PICC_knownset_get_element(ks, val_known2);
    elem->state = PICC_KNOWN;

    PICC_KnownSet *subset = PICC_knownset_known(ks);
    ASSERT(PICC_knownset_size(subset) == 2);
    ASSERT(subset->content[0].value == val_known1);
    ASSERT(subset->content[1].value == val_known2);
}

void test_forget_subset(PICC_Error *error)
{
}

/**
 * Runs all known set tests.
 */
void PICC_test_knownset()
{
    ALLOC_ERROR(error);
    test_knownset_creation(&error);
    test_register(&error);
    test_add(&error);
    test_known_subset(&error);
    test_forget_subset(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

