#include <stddef.h>
#include <stdint.h>

void* memset(void* destination, int c, size_t length)
{
    const unsigned long vector = 0x0101010101010101UL * (unsigned char) c;
    unsigned char* dst = destination;

    for (; length && (uintptr_t) dst % sizeof(unsigned long); --length)
        *dst++ = c;

    unsigned long* vdst = (unsigned long*) destination;

    for (; length >= sizeof(unsigned long); length -= sizeof(unsigned long))
        *vdst++ = vector;

    uint32_t* dst32 = (uint32_t*) vdst;

    if (sizeof(unsigned long) > sizeof(uint32_t) && length & sizeof(uint32_t))
        *dst32++ = vector;

    uint16_t* dst16 = (uint16_t*) dst32;

    if (length & sizeof(uint16_t))
        *dst16++ = vector;

    dst = (unsigned char*) dst16;

    if (length & 1)
        *dst = c;

    return destination;
}
