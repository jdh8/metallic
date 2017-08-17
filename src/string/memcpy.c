#include "copy.h"

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    copy(destination, source, length);
    return destination;
}
