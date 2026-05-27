#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int wprintf(const wchar_t* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vfwprintf(stdout, format, list);
    va_end(list);
    return r;
}
