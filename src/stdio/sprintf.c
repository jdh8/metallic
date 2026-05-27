#include <stdio.h>
#include <stdarg.h>

int sprintf(char s[restrict static 1], const char format[restrict static 1], ...)
{
    va_list list;
    va_start(list, format);

    int status = vsprintf(s, format, list);
    va_end(list);

    return status;
}
