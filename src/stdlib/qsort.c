#include "../string/copy.h"
#include <stddef.h>
#include <stdint.h>

static void insertion_sort_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    size_t alignment = (uintptr_t)data | size;
    char* begin = data;

    for (size_t i = 1; i < count; ++i) {
        char c[size];
        char* a = begin + i * size;
        char* b;

        copy_(c, a, size, alignment);

        for (b = a - size; b >= begin && compare(c, b) < 0; b -= size)
            copy_(b + size, b, size, alignment);

        copy_(b + size, c, size, alignment);
    }
}

#define MEMSWAP(T) (T a[restrict], T b[restrict], size_t count) \
{                                                               \
    for (size_t i = 0; i < count; ++i) {                        \
        T c = a[i];                                             \
        a[i] = b[i];                                            \
        b[i] = c;                                               \
    }                                                           \
}

static void swap64_ MEMSWAP(uint64_t);
static void swap32_ MEMSWAP(uint32_t);
static void swap16_ MEMSWAP(uint16_t);
static void swap8_ MEMSWAP(unsigned char);

static void swap_(void* restrict a, void* restrict b, size_t size, size_t alignment)
{
    switch (alignment & -alignment) {
        top:
            return swap64_(a, b, size >> 3);
        case 4:
            return swap32_(a, b, size >> 2);
        case 2:
            return swap16_(a, b, size >> 1);
        case 1:
            break;
        default:
            goto top;
    }

    return swap8_(a, b, size);
}

static size_t leaf_(void* data, size_t i, size_t count, size_t size, int compare(const void*, const void*))
{
    while (2 * i < count) {
        char* base = data;
        i = 2 * i | (compare(base + (2 * i - 1) * size, base + 2 * i * size) < 0);
    }
    return i << (2 * i == count);
}

static void siftdown_(void* data, size_t stem, size_t count, size_t size, int compare(const void*, const void*))
{
    size_t alignment = (uintptr_t)data | size;
    size_t offspring = leaf_(data, stem, count, size, compare);
    char* base = data;
    char buffer[size];

    while (compare(base + size * (offspring - 1), base + size * (stem - 1)) < 0)
        offspring >>= 1;

    copy_(buffer, base + size * (offspring - 1), size, alignment);
    copy_(base + size * (offspring - 1), base + size * (stem - 1), size, alignment);

    while (stem < offspring) {
        offspring >>= 1;
        swap_(buffer, base + size * (offspring - 1), size, alignment);
    }
}

static void heapsort_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    size_t alignment = (uintptr_t)data | size;

    for (size_t parent = count >> 1; parent; --parent)
        siftdown_(data, parent, count, size, compare);

    while (count--) {
        swap_(data, (char*)data + count * size, size, alignment);
        siftdown_(data, 1, count, size, compare);
    }
}

static void* partition_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    size_t alignment = (uintptr_t)data | size;
    char* front = data;
    char* middle = front + size * (count >> 1);
    char* back = front + size * (count - 1);

    if (compare(middle, front) < 0)
        swap_(front, middle, size, alignment);

    if (compare(back, front) < 0)
        swap_(front, back, size, alignment);

    if (compare(middle, back) < 0)
        swap_(middle, back, size, alignment);

    void* pivot = back;

    for (;; swap_(front, back, size, alignment)) {
        while (compare(front += size, pivot) < 0);
        while (compare(pivot, back -= size) < 0);

        if (front >= back) {
            swap_(front, pivot, size, alignment);
            return front;
        }
    }
}

static void introsort_(void* data, size_t count, size_t size, int ttl, int compare(const void*, const void*))
{
    if (count <= 8)
        return insertion_sort_(data, count, size, compare);

    if (ttl < 0)
        return heapsort_(data, count, size, compare);

    void* pivot = partition_(data, count, size, compare);
    size_t index = ((char*)pivot - (char*)data) / size;

    introsort_(data, index, size, ttl - 1, compare);
    introsort_(pivot, count - index, size, ttl - 1, compare);
}

void qsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    int ttl = __builtin_clz(1) - __builtin_clz(count);

    introsort_(data, count, size, ttl, compare);
}
