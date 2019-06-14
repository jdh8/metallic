#include <stddef.h>
#include <stdint.h>

#define COPY(T) (T* destination, const T* source, size_t count) \
{                                                               \
    for (size_t i = 0; i < count; ++i)                          \
        destination[i] = source[i];                             \
                                                                \
    return destination;                                         \
}

static void* copy64_ COPY(uint_least64_t);
static void* copy32_ COPY(uint_least32_t);
static void* copy16_ COPY(uint_least16_t);
static void* copy8_ COPY(unsigned char);

static void* copy_(void* destination, const void* source, size_t size, size_t alignment)
{
    switch (alignment & -alignment) {
        top:
            return copy64_(destination, source, size >> 3);
        case 4:
            return copy32_(destination, source, size >> 2);
        case 2:
            return copy16_(destination, source, size >> 1);
        case 1:
            return copy8_(destination, source, size);
        default:
            goto top;
    }
}
