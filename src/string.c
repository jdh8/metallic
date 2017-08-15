#include <stddef.h>
#include <stdint.h>

void* memchr(const void* source, int c, size_t length)
{
    for (const unsigned char* cache = source; length; --length)
    {
        if (*cache == c)
            return (void*) cache;
        else
            ++cache;
    }

    return NULL;
}

static int compare(int a, int b)
{
    return (a > b) - (a < b);
}

int memcmp(const void* lhs, const void* rhs, size_t length)
{
    const unsigned char* a = lhs;
    const unsigned char* b = rhs;

    for (int sign; length; --length)
    {
        if ((sign = compare(*a++, *b++)))
            return sign;
    }

    return 0;
}

void* memset(void* destination, int c, size_t length)
{
    const uint64_t mask = UINT64_C(0x0101010101010101) * (unsigned char) c;
    unsigned char* dst = destination;

    /* Align destination by 8 bytes */
    for (; length && (uintptr_t) dst % 8; --length)
        *dst++ = c;

    /* Set aligned 8-byte chunks */
    for (; length >= 8; length -= 8)
    {
        *(uint64_t*) dst = mask;
        dst += 8;
    }

    /* Set remaining bytes */

    if (length & 4)
    {
        *(uint32_t*) dst = mask;
        dst += 4;
    }

    if (length & 2)
    {
        *(uint16_t*) dst = mask;
        dst += 2;
    }

    if (length & 1)
        *dst = c;

    return destination;
}

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    unsigned char* restrict dst = destination;
    const unsigned char* restrict src = source;

    /* Align destination by 8 bytes */
    for (; length && (uintptr_t) dst % 8; --length)
        *dst++ = *src++;

    /* Copy aligned 8-byte chunks */
    for (; length >= 8; length -= 8)
    {
        *(uint64_t*) dst = *(const uint64_t*) src;
        dst += 8;
        src += 8;
    }

    /* Copy remaining bytes */

    if (length & 4)
    {
        *(uint32_t*) dst = *(const uint32_t*) src;
        dst += 4;
        src += 4;
    }

    if (length & 2)
    {
        *(uint16_t*) dst = *(const uint16_t*) src;
        dst += 2;
        src += 2;
    }

    if (length & 1)
        *dst = *src;

    return destination;
}

void* memmove(void* destination, const void* source, size_t length)
{
    unsigned char* restrict dst = destination;
    const unsigned char* restrict src = source;

    if (dst - src >= length)
    {
        for (; length; --length)
            *dst++ = *src++;
    }
    else
    {
        dst += length;
        src += length;

        for (; length; --length)
            *--dst = *--src;
    }

    return destination;
}
