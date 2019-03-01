#include "stdout.h"
#include <string.h>
#include <stdio.h>

int puts(const char s[static 1])
{
    __stdout(s, strlen(s), (void*)0);
    return putchar('\n');
}
