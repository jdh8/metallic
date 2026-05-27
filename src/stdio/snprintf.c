#include <stdio.h>
#include <stdarg.h>

int snprintf(char* restrict buffer, size_t size, const char format[restrict static 1], ...)
{
    va_list list;
    va_start(list, format);

    int status = vsnprintf(buffer, size, format, list);
    va_end(list);

    return status;
}
