#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int swscanf(const wchar_t* restrict ws, const wchar_t* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vswscanf(ws, format, list);
    va_end(list);
    return r;
}
