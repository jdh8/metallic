#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int wscanf(const wchar_t* restrict format, ...)
{
    va_list list;
    va_start(list, format);
    int r = vfwscanf(stdin, format, list);
    va_end(list);
    return r;
}
