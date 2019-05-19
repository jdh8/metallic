#include <stdio.h>
#include <stdarg.h>

int fprintf(FILE* restrict stream, const char format[restrict static 1], ...)
{
    va_list list;
    va_start(list, format);

    int status = vfprintf(stream, format, list);
    va_end(list);

    return status;
}
