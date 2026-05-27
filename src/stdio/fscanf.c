#include <stdio.h>
#include <stdarg.h>

int fscanf(FILE* restrict stream, const char* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vfscanf(stream, format, list);
    va_end(list);
    return r;
}
