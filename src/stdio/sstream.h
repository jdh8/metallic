#include "FILE.h"

size_t __sstream_read(void* restrict, size_t, FILE[restrict static 1]);
size_t __sstream_write(const void* restrict, size_t, FILE[restrict static 1]);

static FILE sstream_(const char* s)
{
    return (FILE){
        .base = (unsigned char*)s,
        .ptr = (unsigned char*)s,
        .read = __sstream_read,
        .write = __sstream_write,
    };
}
