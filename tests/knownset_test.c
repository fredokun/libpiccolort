/**
 * @file knownset_test.c
 * Unit testing of known set
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
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


/**
 * Runs all known set tests.
 */
void PICC_test_knownset()
{
    ALLOC_ERROR(error);
    test_knownset_creation(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

