/**
 * @file runtime_tests.c
 * Runtime unit tests
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Joel HING
 */

#include <stdio.h>
#include <stdlib.h>
#include <runtime.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))


/**
 * Runs all runtime tests.
 */
void PICC_test_runtime()
{
    ALLOC_ERROR(error);
    //...

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
