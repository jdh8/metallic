#include "aliased.h"
#include <stddef.h>
#include <stdint.h>

void* memchr(const void* source, int character, size_t length)
{
    const unsigned char* input = source;
    unsigned char c = character;

    for (; (uintptr_t)input % sizeof(uint64_t) && length--; ++input)
        if (*input == c)
            return (unsigned char*)input;

    const uint64_t ALIASED* vector = (const uint64_t*)input;
    const uint64_t magic = 0x7EFEFEFEFEFEFEFF;
    uint64_t mask = 0x0101010101010101u * c;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t)) {
        uint64_t word = *vector ^ mask;

        if (((word + magic) ^ ~word) & ~magic)
            for (int k = 0; k < sizeof(uint64_t); ++k)
                if (input[k] == c)
                    return (unsigned char*)(input + k);

        input = (const unsigned char*)++vector;
    }

    for (; length--; ++input)
        if (*input == c)
            return (unsigned char*)input;

    return 0;
}
