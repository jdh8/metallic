#include <wchar.h>
#include <stdio.h>

int vwscanf(const wchar_t* restrict format, __builtin_va_list list)
{
    return vfwscanf(stdin, format, list);
}
