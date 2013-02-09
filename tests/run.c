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
#include <tests.h>

int main(int argc, char **argv)
{
    printf("== Run unit tests suite ==\n\n");

    printf("Run pi_thread tests...\n");
    PICC_test_pithread();

    printf("Run runtime tests...\n");
    PICC_test_runtime();

    printf("Run queue tests...\n");
    PICC_test_queue();

    printf("Run channel tests...\n");
    PICC_test_channel();

    return 0;
}
