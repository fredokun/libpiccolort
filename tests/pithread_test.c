/**
 * @file runtime_tests.c
 * File that contains all the necesseray tests to check the behavior of all functions of runtime.c
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Joel HING
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <pi_thread.h>


//int PICC_GC2(PICC_SchedPool schedpool);

/**
 * Test : PICC_create_pithread \n
 * PICC_PiThread *PICC_create_pithread();
 *
 * @return boolean true if it works else false
 */
bool check_pithread(PICC_Error *error)
{

    PICC_PiThread *p = PICC_create_pithread(1, 1);
    if (p == NULL) {
        NEW_ERROR(error,ERR_NULLPOINTER_PITHREAD);
    } else {
        free(p);
        return true;
    }

    return false;
}
