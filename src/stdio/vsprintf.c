#include "FILE.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static size_t write_(FILE stream[restrict static 1], const void* restrict buffer, size_t size)
{
    memcpy(stream->ptr, buffer, size);
    stream->ptr += size;
    return size;
}

int vsprintf(char s[restrict static 1], const char format[restrict static 1], va_list list)
{
    FILE stream = {
        .ptr = (unsigned char*)s,
        .write = write_,
    };

    int status = vfprintf(&stream, format, list);
    *stream.ptr = '\0';
    return status;
}
