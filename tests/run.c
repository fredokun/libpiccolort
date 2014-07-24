/**
 * @file main.c
 * Runs all unit tests.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Mickaël MENU
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tests.h"

int main(int argc, char **argv)
{
    printf("== Run unit tests suite ==\n\n");

    printf("Run pi_thread tests...\n");
    PICC_test_pithread();

    printf("Run queue tests...\n");
    PICC_test_queue();

    printf("Run channel tests...\n");
    PICC_test_channel();

    printf("Run commit tests...\n");
    PICC_test_commit();

    printf("Run atomic tests...\n");
    PICC_test_atomic();

    printf("Run value tests...\n");
    PICC_test_value();

    printf("Run known set tests...\n");
    PICC_test_knownset();

    return 0;
}
