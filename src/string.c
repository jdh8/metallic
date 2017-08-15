#include <stddef.h>
#include <stdint.h>

void* memchr(const void* source, int c, size_t length)
{
    for (const unsigned char* cache = source; length; --length)
        if (*cache == c)
            return (void*) cache;
        else
            ++cache;

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
        if ((sign = compare(*a++, *b++)))
            return sign;

    return 0;
}

void* memset(void* destination, int c, size_t length)
{
    const uint64_t vector = UINT64_C(0x0101010101010101) * (unsigned char) c;
    unsigned char* dst = destination;

    for (; length && (uintptr_t) dst % sizeof(uint64_t); --length)
        *dst++ = c;

    uint64_t* vdst = (uint64_t*) destination;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *vdst++ = vector;

    uint32_t* dst32 = (uint32_t*) vdst;

    if (length & sizeof(uint32_t))
        *dst32++ = vector;

    uint16_t* dst16 = (uint16_t*) dst32;

    if (length & sizeof(uint16_t))
        *dst16++ = vector;

    dst = (unsigned char*) dst16;

    if (length & 1)
        *dst = c;

    return destination;
}

static unsigned char* copy(unsigned char* destination, const unsigned char* source, size_t length)
{
    for (; length && (uintptr_t) destination % sizeof(uint64_t); --length)
        *destination++ = *source++;

    uint64_t* vdst = (uint64_t*) destination;
    const uint64_t* vsrc = (const uint64_t*) source;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *vdst++ = *vsrc++;

    uint32_t* dst32 = (uint32_t*) vdst;
    const uint32_t* src32 = (const uint32_t*) vsrc;

    if (length & sizeof(uint32_t))
        *dst32++ = *src32++;

    uint16_t* dst16 = (uint16_t*) dst32;
    const uint16_t* src16 = (const uint16_t*) src32;

    if (length & sizeof(uint16_t))
        *dst16++ = *src16++;

    destination = (unsigned char*) dst16;
    source = (const unsigned char*) src16;

    if (length & 1)
        *destination++ = *source++;

    return destination;
}

void* memcpy(void* restrict destination, const void* restrict source, size_t length)
{
    copy(destination, source, length);
    return destination;
}

static unsigned char* reverse_copy(unsigned char* destination, const unsigned char* source, size_t length)
{
    destination += length;
    source += length;

    for (; length && (uintptr_t) destination % sizeof(uint64_t); --length)
        *--destination = *--source;

    uint64_t* vdst = (uint64_t*) destination;
    const uint64_t* vsrc = (const uint64_t*) source;

    for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t))
        *--vdst = *--vsrc;

    uint32_t* dst32 = (uint32_t*) vdst;
    const uint32_t* src32 = (const uint32_t*) vsrc;

    if (length & sizeof(uint32_t))
        *--dst32 = *--src32;

    uint16_t* dst16 = (uint16_t*) dst32;
    const uint16_t* src16 = (const uint16_t*) src32;

    if (length & sizeof(uint16_t))
        *--dst16 = *--src16;

    destination = (unsigned char*) dst16;
    source = (const unsigned char*) src16;

    if (length & 1)
        *--destination = *--source;

    return destination;
}

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
