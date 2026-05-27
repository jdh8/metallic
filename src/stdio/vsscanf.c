#include "FILE.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static size_t read_(FILE stream[restrict static 1], void* restrict buffer, size_t size)
{
    size_t avail = (size_t)(stream->end - stream->ptr);
    size_t n = size < avail ? size : avail;
    if (!n)
        return 0;
    memcpy(buffer, stream->ptr, n);
    stream->ptr += n;
    return n;
}

int vsscanf(const char* restrict s, const char* restrict format, va_list list)
{
    size_t len = strlen(s);
    FILE stream = {
        .ptr = (unsigned char*)s,
        .end = (unsigned char*)s + len,
        .read = read_,
    };

    return vfscanf(&stream, format, list);
}
