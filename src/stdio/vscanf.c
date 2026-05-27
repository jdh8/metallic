#include <stdio.h>

int vscanf(const char* restrict format, __builtin_va_list list)
{
    return vfscanf(stdin, format, list);
}
