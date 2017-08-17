#include <stddef.h>
#include <stdint.h>

void* memchr(const void* source, int c, size_t length)
{
    const unsigned char* src = source;

    for (; length && (uintptr_t) src % sizeof(unsigned long); --length)
        if (*src == c)
            return (void*) src;
        else
            ++src;

    const unsigned long* vsrc = (const unsigned long*) src;
    const unsigned long magic = 0x7efefefefefefeffUL;
    const unsigned long mask = 0x0101010101010101UL * (unsigned char) c;

    for (; length >= sizeof(unsigned long); length -= sizeof(unsigned long))
    {
        src = (const unsigned char*) vsrc;
        unsigned long word = *vsrc++ ^ mask;

        if (((word + magic) ^ ~word) & ~magic)
            for (int k = 0; k < sizeof(unsigned long); ++k)
                if (src[k] == c)
                    return (void*)(src + k);
    }

    for (; length; --length)
        if (*src == c)
            return (void*) src;
        else
            ++src;

    return NULL;
}
