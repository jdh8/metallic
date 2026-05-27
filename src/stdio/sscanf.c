#include <stdio.h>
#include <stdarg.h>

int sscanf(const char* restrict s, const char* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vsscanf(s, format, list);
    va_end(list);
    return r;
}
