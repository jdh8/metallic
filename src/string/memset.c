#include "aliased.h"
#include <stddef.h>
#include <stdint.h>

void* memset(void* destination, int character, size_t length)
{
    unsigned char* output = destination;
    unsigned char c = character;
    uint64_t vector = 0x0101010101010101u * c;

    while ((uintptr_t)output % sizeof(uint64_t) && length--)
        *output++ = c;

    uint64_t ALIASED* alias = (uint64_t*)output;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *alias++ = vector;

    output = (unsigned char*)alias;

    while (length--)
        *output++ = c;

    return destination;
}
