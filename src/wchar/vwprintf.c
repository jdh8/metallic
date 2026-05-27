#include <wchar.h>
#include <stdio.h>

int vwprintf(const wchar_t* restrict format, __builtin_va_list list)
{
    return vfwprintf(stdout, format, list);
}
