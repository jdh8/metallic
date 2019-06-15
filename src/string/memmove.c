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

static void* rcopy64_ REVERSE_COPY(uint_least64_t)
static void* rcopy32_ REVERSE_COPY(uint_least32_t)
static void* rcopy16_ REVERSE_COPY(uint_least16_t)
static void* rcopy8_ REVERSE_COPY(unsigned char)

static void* reverse_copy_(void* destination, const void* source, size_t size, size_t alignment)
{
    switch (alignment & -alignment) {
        top:
            return rcopy64_(destination, source, size >> 3);
        case 4:
            return rcopy32_(destination, source, size >> 2);
        case 2:
            return rcopy16_(destination, source, size >> 1);
        case 1:
            return rcopy8_(destination, source, size);
        default:
            goto top;
    }
}

void* memmove(void* destination, const void* source, size_t length)
{
    size_t alignment = (uintptr_t)destination | (uintptr_t)source | length;

    if ((char*)destination - (const char*)source < length)
        return reverse_copy_(destination, source, length, alignment);

    return copy_(destination, source, length, alignment);
}
