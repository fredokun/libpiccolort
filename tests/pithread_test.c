/**
 * @file pithread_tests.c
 * PiThread unit tests.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Joel HING
 */

#include <stdio.h>
#include <stdlib.h>
#include <pi_thread_repr.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))

/**
 * Test : PICC_create_pithread \n
 * PICC_PiThread *PICC_create_pithread();
 *
 * @return boolean true if it works else false
 */
void test_create_pithread(PICC_Error *error)
{
    PICC_PiThread *p = PICC_create_pithread(1, 1, 1);
    ASSERT(p != NULL);
}

/**
 * Runs all PiThread tests.
 */
void PICC_test_pithread()
{
    ALLOC_ERROR(error);
    test_create_pithread(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
