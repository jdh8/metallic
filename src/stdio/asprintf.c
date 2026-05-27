#include <stdio.h>
#include <stdarg.h>

int asprintf(char** restrict out, const char* restrict format, ...)
{
    va_list list;
    va_start(list, format);

    int n = vasprintf(out, format, list);
    va_end(list);

    return n;
}
