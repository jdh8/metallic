#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int swprintf(wchar_t* restrict buf, size_t cap, const wchar_t* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vswprintf(buf, cap, format, list);
    va_end(list);
    return r;
}
