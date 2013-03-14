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

/*
PICC_KnownSet* PICC_create_empty_knownset();
PICC_KnownSet* PICC_create_knownset(int size, PICC_Error* error);
*
bool PICC_knownset_add_tree(PICC_KnownSet *s, GEN_VALUE elem);
bool PICC_knownset_add_list(PICC_KnownSet *s, GEN_VALUE elem);
bool PICC_knownset_add(PICC_KnownSet *s, GEN_VALUE elem);
bool PICC_known_set_mem_tree(PICC_KnownSet *ss, GEN_VALUE elem);
bool PICC_known_set_mem_list(PICC_KnownSet *ss, GEN_VALUE elem);
bool PICC_known_set_mem(PICC_KnownSet *s, GEN_VALUE elem);
int PICC_knownset_size_tree(PICC_KnownSet *ss);
int PICC_knownset_size_list(PICC_KnownSet *ss);
int PICC_knownset_size(PICC_KnownSet *s);
bool PICC_known_set_compare_tree(PICC_KnownSet *s, PICC_KnownSet *s2);
bool PICC_known_set_compare_list(PICC_KnownSet *s, PICC_KnownSet *s2);
bool PICC_known_set_compare(PICC_KnownSet *s, PICC_KnownSet *s2);
PICC_KnownSetIterator *PICC_create_knownset_iterator(PICC_KnownSet *s);
PICC_KnownSetIterator *PICC_delete_known_set_iterator(PICC_KnownSetIterator *it);
GEN_VALUE PICC_known_set_next(PICC_KnownSetIterator *it);
bool PICC_known_set_has_next(PICC_KnownSetIterator *it);
PICC_KnownSetTreeIterator *PICC_create_knownset_tree_iterator(PICC_KnownSetTree *s);
PICC_KnownSetTreeIterator *PICC_delete_known_set_tree_iterator(PICC_KnownSetTreeIterator *it);
GEN_VALUE PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it, bool check);
bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetTreeIterator *it);
PICC_KnownSetListIterator *PICC_create_knownset_list_iterator(PICC_KnownSetList *s);
PICC_KnownSetListIterator *PICC_delete_known_set_list_iterator(PICC_KnownSetListIterator *it);
GEN_VALUE PICC_known_set_list_iterator_next(PICC_KnownSetListIterator *it);
bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator *it);
*/

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

void test_tree(PICC_Error *error)
{
}

/**
 * Runs all known set tests.
 */
void PICC_test_knownset()
{
    ALLOC_ERROR(error);
    test_knownset_creation(&error);
    test_tree(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

