#include <stdio.h>
#include <stdlib.h>
#include <tools.h>

void debug(const char* s)
{
    #ifdef DEBUG
        printf(s);
    #endif
}
