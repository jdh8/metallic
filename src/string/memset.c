#include <stddef.h>
#include <stdint.h>

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
