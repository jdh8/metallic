#include "../string/copy.h"
#include <stddef.h>
#include <stdint.h>

static void insertion_sort_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    char* begin = data;

    for (size_t i = 1; i < count; ++i) {
        char c[size];
        char* a = begin + i * size;
        char* b;

        copy_(c, a, size);

        for (b = a - size; b >= begin && compare(c, b) < 0; b -= size)
            copy_(b + size, b, size);

        copy_(b + size, c, size);
    }
}

#define SWAP(T) (T a[restrict], T b[restrict], size_t count) \
{                                                            \
    for (size_t i = 0; i < count; ++i) {                     \
        T c = a[i];                                          \
        a[i] = b[i];                                         \
        b[i] = c;                                            \
    }                                                        \
}

static void swap0_ SWAP(uint8_t)
static void swap1_ SWAP(uint16_t)
static void swap2_ SWAP(uint32_t)
static void swap3_ SWAP(uint64_t)

static void swap_(void* restrict a, void* restrict b, size_t size)
{
    switch (size & -size) {
        case 1:
            swap0_(a, b, size);
            break;
        case 2:
            swap1_(a, b, size >> 1);
            break;
        case 4:
            swap2_(a, b, size >> 2);
            break;
        default:
            swap3_(a, b, size >> 3);
    }
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
    size_t offspring = leaf_(data, stem, count, size, compare);
    char* base = data;
    char buffer[size];

    while (compare(base + size * (offspring - 1), base + size * (stem - 1)) < 0)
        offspring >>= 1;

    copy_(buffer, base + size * (offspring - 1), size);
    copy_(base + size * (offspring - 1), base + size * (stem - 1), size);

    while (stem < offspring) {
        offspring >>= 1;
        swap_(buffer, base + size * (offspring - 1), size);
    }
}

static void heapsort_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    for (size_t parent = count >> 1; parent; --parent)
        siftdown_(data, parent, count, size, compare);

    while (count--) {
        swap_(data, (char*)data + count * size, size);
        siftdown_(data, 1, count, size, compare);
    }
}

static void* partition_(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    char* front = data;
    char* middle = front + size * (count >> 1);
    char* back = front + size * (count - 1);

    if (compare(middle, front) < 0)
        swap_(front, middle, size);

    if (compare(back, front) < 0)
        swap_(front, back, size);

    if (compare(middle, back) < 0)
        swap_(middle, back, size);

    void* pivot = back;

    while (1) {
        while (compare(front += size, pivot) < 0);
        while (compare(pivot, back -= size) < 0);

        if (front >= back) {
            swap_(front, pivot, size);
            return front;
        }
        swap_(front, back, size);
    }
}

static void introsort_(void* data, size_t count, size_t size, int ttl, int compare(const void*, const void*))
{
    if (count <= 8)
        insertion_sort_(data, count, size, compare);
    else if (ttl < 0)
        heapsort_(data, count, size, compare);
    else {
        void* pivot = partition_(data, count, size, compare);
        size_t index = ((char*)pivot - (char*)data) / size;

        introsort_(data, index, size, ttl - 1, compare);
        introsort_(pivot, count - index, size, ttl - 1, compare);
    }
}

void qsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    int ttl = __builtin_clz(1) - __builtin_clz(count);

    introsort_(data, count, size, ttl, compare);
}
