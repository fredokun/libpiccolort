#include <stdlib.h>
#include <stdio.h>
#include <commit.h>
#include <channel.h>
#include <set.h>

int main()
{
    PICC_Set* commit_set = PICC_set_make(PICC_COMMIT);
    //PICC_Set* knowns_set = PICC_set_make(PICC_KNOWNS);

    PICC_Commit* c = PICC_create_commitment(NULL);
    PICC_Commit* c2 = PICC_create_commitment(NULL);
    PICC_Commit* c3 = PICC_create_commitment(NULL);

    PICC_SET_ADD(commit_set, c);
    PICC_SET_ADD(commit_set, c2);

    return 0;
}
