#include <stdio.h>
#include <stdarg.h>

int vprintf(const char format[restrict static 1], va_list list) 
{
    return vfprintf(stdout, format, list);
}
