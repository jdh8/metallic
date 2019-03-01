#include "copy.h"
#include <stddef.h>
#include <stdint.h>

#define REVERSE_COPY(T) (T* destination, const T* source, size_t count) \
{                                                                       \
    while (--count != -1)                                               \
        destination[count] = source[count];                             \
                                                                        \
    return destination;                                                 \
}

static void* _rcopy64 REVERSE_COPY(uint_least64_t);
static void* _rcopy32 REVERSE_COPY(uint_least32_t);
static void* _rcopy16 REVERSE_COPY(uint_least16_t);
static void* _rcopy8 REVERSE_COPY(unsigned char);

static void* _reverse_copy(void* destination, const void* source, size_t size, size_t alignment)
{
    switch (alignment & -alignment) {
        top:
            return _rcopy64(destination, source, size >> 3);
        case 4:
            return _rcopy32(destination, source, size >> 2);
        case 2:
            return _rcopy16(destination, source, size >> 1);
        case 1:
            return _rcopy8(destination, source, size);
        default:
            goto top;
    }
}

void* memmove(void* destination, const void* source, size_t length)
{
    size_t alignment = (uintptr_t)destination | (uintptr_t)source | length;

    if ((char*)destination - (const char*)source < length)
        return _reverse_copy(destination, source, length, alignment);

    return _copy(destination, source, length, alignment);
}
