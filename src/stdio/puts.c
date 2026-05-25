#include <string.h>
#include <stdio.h>

size_t __stdio_write(FILE* restrict, const void* restrict, size_t);

int puts(const char s[static 1])
{
    __stdio_write(stdout, s, strlen(s));

    return putchar('\n');
}
