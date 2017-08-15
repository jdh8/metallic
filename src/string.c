#include <stddef.h>

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    char* restrict dst = destination;
    const char* restrict src = source;

    /* I know this is naive, but compiler will optimized this because we have restrict. */
    for (; length; --length)
        *dst++ = *src++;

    return destination;
}
