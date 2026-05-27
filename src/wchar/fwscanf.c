#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int fwscanf(FILE* restrict stream, const wchar_t* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vfwscanf(stream, format, list);
    va_end(list);
    return r;
}
