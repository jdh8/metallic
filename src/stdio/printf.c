#include <stdio.h>
#include <stdarg.h>

int printf(const char format[restrict static 1], ...)
{
    va_list list;
    va_start(list, format);

    int status = vfprintf(stdout, format, list);
    va_end(list);

    return status;
}
