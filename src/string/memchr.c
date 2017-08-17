#include <stddef.h>
#include <stdint.h>

void* memchr(const void* source, int c, size_t length)
{
    const unsigned char* src = source;

    for (; length && (uintptr_t) src % sizeof(uint64_t); --length)
        if (*src++ == c)
            return (void*)--src;

    const uint64_t* vsrc = (const uint64_t*) src;
    const uint64_t magic = 0x7efefefefefefeffu;
    const uint64_t mask = 0x0101010101010101u * (unsigned char) c;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
    {
        src = (const unsigned char*) vsrc;
        uint64_t word = *vsrc++ ^ mask;

        if (((word + magic) ^ ~word) & ~magic)
            for (int k = 0; k < sizeof(uint64_t); ++k)
                if (src[k] == c)
                    return (void*)(src + k);
    }

    for (; length; --length)
        if (*src++ == c)
            return (void*)--src;

    return NULL;
}
