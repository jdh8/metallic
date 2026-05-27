#include <stdio.h>
#include <stdarg.h>

int scanf(const char* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vfscanf(stdin, format, list);
    va_end(list);
    return r;
}
