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

#define DEBUG

int main()
{
    PICC_test_generic_sets();
    printf("run tests channel\n");
    PICC_test_channels();

    return 0;
}
