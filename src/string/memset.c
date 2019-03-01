#include <stddef.h>
#include <stdint.h>

void* memset(void* destination, int character, size_t length)
{
    unsigned char* output = destination;
    unsigned char c = character;
    uint64_t vector = 0x0101010101010101u * c;

    while ((uintptr_t)output % sizeof(uint64_t) && length--)
        *output++ = c;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t)) {
        *(uint64_t*)output = vector;
        output += sizeof(uint64_t);
    }

    while (length--)
        *output++ = c;

    return destination;
}
