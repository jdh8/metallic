#include <stddef.h>
#include <stdint.h>

#define COPY(T) (T* destination, const T* source, size_t count) \
{                                                               \
    for (size_t i = 0; i < count; ++i)                          \
        destination[i] = source[i];                             \
                                                                \
    return destination;                                         \
}

static void* copy0_ COPY(uint8_t)
static void* copy1_ COPY(uint16_t)
static void* copy2_ COPY(uint32_t)
static void* copy3_ COPY(uint64_t)

static void* copy_(void* destination, const void* source, size_t size)
{
    switch (size & -size) {
        case 1:
            return copy0_(destination, source, size);
        case 2:
            return copy1_(destination, source, size >> 1);
        case 4:
            return copy2_(destination, source, size >> 2);
        default:
            return copy3_(destination, source, size >> 3);
    }
}
