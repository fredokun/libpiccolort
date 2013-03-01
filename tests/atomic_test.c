/**
 * @file atomic_test.c
 * Unit testing of atomic values
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <atomic_repr.h>
#include <error.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))

void test_creation(PICC_Error *error)
{
    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_bool_get(abool) == false);

    PICC_AtomicInt *aint = PICC_create_atomic_int(error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_int_get(aint) == 0);

    PICC_free_atomic_bool(abool);
    PICC_free_atomic_int(aint);
}

void test_compare_and_swap(PICC_Error *error)
{
    bool bret;
    int iret;

    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_bool_get(abool) == false);

    bret = PICC_atomic_bool_compare_and_swap(abool, false, true);
    ASSERT(bret == false);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    bret = PICC_atomic_bool_compare_and_swap(abool, false, false);
    ASSERT(bret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    bret = PICC_atomic_bool_compare_and_swap(abool, true, true);
    ASSERT(bret == true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    PICC_AtomicInt *aint = PICC_create_atomic_int(error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_atomic_int_get(aint) == 0);

    iret = PICC_atomic_int_val_compare_and_swap(aint, 0, 2);
    ASSERT(iret == 0);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    iret = PICC_atomic_int_val_compare_and_swap(aint, 1, 3);
    ASSERT(iret == 2);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    iret = PICC_atomic_int_val_compare_and_swap(aint, 2, 2);
    ASSERT(iret == 2);
    ASSERT(PICC_atomic_int_get(aint) == 2);

    PICC_free_atomic_bool(abool);
    PICC_free_atomic_int(aint);
}

void test_get_and_set(PICC_Error *error)
{
    PICC_AtomicBoolean *abool = PICC_create_atomic_bool(error);
    ASSERT_NO_ERROR();
    PICC_AtomicInt *aint = PICC_create_atomic_int(error);
    ASSERT_NO_ERROR();

    PICC_atomic_bool_set(abool, true);
    ASSERT(PICC_atomic_bool_get(abool) == true);

    PICC_atomic_int_set(aint, 3);
    ASSERT(PICC_atomic_int_get(aint) == 3);
}

/**
 * Runs all atomic values tests.
 */
void PICC_test_atomic()
{
    ALLOC_ERROR(error);
    test_creation(&error);
    test_compare_and_swap(&error);
    test_get_and_set(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

