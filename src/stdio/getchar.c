#include "getc.h"
#include <stdio.h>

int getchar(void)
{
    return getc_(stdin);
}
