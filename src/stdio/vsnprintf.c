#include "FILE.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static size_t noop_(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    return size;
}

static size_t write_(const void* restrict buffer, size_t size, FILE stream[restrict static 1])
{
    size_t space = stream->end - stream->ptr;

    if (size < space) {
        memcpy(stream->ptr, buffer, size);
        stream->ptr += size;
    }
    else {
        memcpy(stream->ptr, buffer, space);
        stream->ptr = stream->end;
        stream->write = noop_;
    }

    return size;
}

int vsnprintf(char* restrict buffer, size_t size, const char format[restrict static 1], va_list list)
{
    FILE stream = {
        .ptr = (unsigned char*)buffer,
        .end = size ? (unsigned char*)buffer + (size - 1) : (void*)0,
        .write = size ? write_ : noop_,
    };

    int status = vfprintf(&stream, format, list);

    if (size)
        *stream.ptr = '\0';

    return status;
}
