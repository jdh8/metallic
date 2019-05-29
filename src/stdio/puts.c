#include <string.h>
#include <stdio.h>
#include <unistd.h>

int puts(const char s[static 1])
{
    write(1, s, strlen(s));

    return putchar('\n');
}
