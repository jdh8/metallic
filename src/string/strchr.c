#include "aliased.h"
#include <stddef.h>
#include <stdint.h>

char* strchr(const char source[static 1], int character)
{
    const unsigned char* input = (const unsigned char*)source;
    unsigned char c = character;

    for (; (uintptr_t)input % sizeof(uint64_t); ++input) {
        if (*input == c)
            return (char*)input;

        if (!*input)
            return 0;
    }

    const uint64_t magic = 0x7EFEFEFEFEFEFEFF;
    uint64_t mask = 0x0101010101010101u * c;

    for (const uint64_t ALIASED* vector = (const uint64_t*)input; ; input = (const unsigned char*)++vector) {
        uint64_t word = *vector ^ mask;

        if (((word + magic) ^ ~word) & ~magic || ((*vector + magic) ^ ~*vector) & ~magic) {
            for (int k = 0; k < sizeof(uint64_t); ++k) {
                if (input[k] == c)
                    return (char*)(input + k);

                if (!input[k])
                    return 0;
            }
        }
    }
}
