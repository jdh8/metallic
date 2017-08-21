#include "copy.h"

void* __mempcpy(void* restrict destination, const void* restrict source, size_t length)
{
    return copy(destination, source, length);
}

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    copy(destination, source, length);
    return destination;
}
