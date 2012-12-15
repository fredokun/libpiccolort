#include <stdio.h>
#include <stdlib.h>
#include <pi_thread.h>
#include <runtime.h>
#include <errors.h>

int main()
{
    /*
    *   Erreurs : initialiser les tableaux d'erreur
    *
    */

    // Première passe
    // + check_pithread (create)
    // + check_commit (commit/commitlist/commitlistelement) (create)
    // + check_channel (create)
    // + check_schedpool (create)
    // + check_schedslave/master (create)
    // Deuxième passe
    // + check_pithread
    // + check_commit (commit/commitlist/commitlistelement)
    // + check_channel
    // + check_schedpool
    // + check_schedslave/master

    return 0;
}
