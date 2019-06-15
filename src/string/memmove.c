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

static void* reverse_copy0_ REVERSE_COPY(uint8_t)
static void* reverse_copy1_ REVERSE_COPY(uint16_t)
static void* reverse_copy2_ REVERSE_COPY(uint32_t)
static void* reverse_copy3_ REVERSE_COPY(uint64_t)

static void* reverse_copy_(void* destination, const void* source, size_t size)
{
    switch (size & -size) {
        case 1:
            return reverse_copy0_(destination, source, size);
        case 2:
            return reverse_copy1_(destination, source, size >> 1);
        case 4:
            return reverse_copy2_(destination, source, size >> 2);
        default:
            return reverse_copy3_(destination, source, size >> 3);
    }
}

void* memmove(void* destination, const void* source, size_t length)
{
    if ((char*)destination - (const char*)source < length)
        return reverse_copy_(destination, source, length);

    return copy_(destination, source, length);
}
