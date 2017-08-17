#include "copy.h"
#include "reverse_copy.h"

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* dst = destination;
    const unsigned char* src = source;

    if (dst - src >= length)
        copy(dst, src, length);
    else
        reverse_copy(dst, src, length);

    return destination;
}
