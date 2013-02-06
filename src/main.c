/**
 * @file main.c
 * Main for generic sets tests.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdlib.h>
#include <stdio.h>

#include <tools.h>
#include <commit.h>
#include <channel.h>
#include <set.h>

//Maxence : tests : need to be refactor (main : runtime / main : test)
#include <pi_thread.h>
#include <runtime.h>
#include <errors.h>
#include <runtime_tests.h>

#define DEBUG

int main()
{
    //PICC_test_generic_sets();
    //printf("run tests channel\n");
    //PICC_test_channels();

    
    ALLOC_ERROR(error);

    check_pithread(&error);
    debug("+ create pithreads :\tOK\n");
    //check_create_channels(error);
    check_create_commitments(&error);
    debug("+ create commitments :\tOK\n");
    check_register_outcommits(&error);
    debug("+ register outcommits :\tOK\n");
    check_register_incommits(&error);
    debug("+ register incommits :\tOK\n");
    check_commitlists(&error);
    debug("+ commitlists functions :\tOK\n");

    return 0;
}
