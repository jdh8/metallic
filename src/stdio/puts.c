#include "fwrite.h"
#include <string.h>
#include <stdio.h>

int puts(const char s[static 1])
{
    _fwrite(s, strlen(s), stdout);

    return putchar('\n');
}
