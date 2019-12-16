#include "sstream.h"
#include <stdio.h>
#include <stdarg.h>

int vsprintf(char* s, const char format[restrict static 1], va_list list) 
{
    FILE stream = sstream_(s);
    int status = vfprintf(&stream, format, list);

    *stream.ptr = '\0';
    return status;
}
