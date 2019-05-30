#include <string.h>
#include <stdio.h>

size_t __stdio_write(const void*, size_t, FILE*);

int puts(const char s[static 1])
{
    __stdio_write(s, strlen(s), stdout);

    return putchar('\n');
}
