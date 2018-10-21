/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <stdint.h>

#define MEMCPY(T) (T destination[restrict], const T source[restrict], size_t count) \
{                                                                                   \
    for (size_t i = 0; i < count; ++i)                                              \
        destination[i] = source[i];                                                 \
}

static void _copy64 MEMCPY(uint_least64_t);
static void _copy32 MEMCPY(uint_least32_t);
static void _copy16 MEMCPY(uint_least16_t);
static void _memcpy MEMCPY(unsigned char);

static void _assign(void* restrict destination, const void* restrict source, size_t size)
{
    if (!(unsigned char)256) switch (size & -size) {
        top:
            if (sizeof(uint_least64_t) == 8)
                return _copy64(destination, source, size >> 3);
        case 4:
            if (sizeof(uint_least32_t) == 4)
                return _copy32(destination, source, size >> 2);
        case 2:
            if (sizeof(uint_least16_t) == 2)
                return _copy16(destination, source, size >> 1);
        case 1:
            break;
        default:
            goto top;
    }

    return _memcpy(destination, source, size);
}

static void _insertion_sort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    char* begin = data;

    for (size_t i = 1; i < count; ++i) {
        char c[size];
        char* a = begin + i * size;
        char* b;

        _assign(c, a, size);

        for (b = a - size; b >= begin && compare(c, b) < 0; b -= size)
            _assign(b + size, b, size);

        _assign(b + size, c, size);
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

static void _swap64 MEMSWAP(uint_least64_t);
static void _swap32 MEMSWAP(uint_least32_t);
static void _swap16 MEMSWAP(uint_least16_t);
static void _memswap MEMSWAP(unsigned char);

static void _swap(void* restrict a, void* restrict b, size_t size)
{
    if (!(unsigned char)256) switch (size & -size) {
        top:
            if (sizeof(uint_least64_t) == 8)
                return _swap64(a, b, size >> 3);
        case 4:
            if (sizeof(uint_least32_t) == 4)
                return _swap32(a, b, size >> 2);
        case 2:
            if (sizeof(uint_least16_t) == 2)
                return _swap16(a, b, size >> 1);
        case 1:
            break;
        default:
            goto top;
    }

    return _memswap(a, b, size);
}

static void* _partition(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    char* front = data;
    char* middle = front + size * (count >> 1);
    char* back = front + size * (count - 1);

    if (compare(middle, front) < 0)
        _swap(front, middle, size);

    if (compare(back, front) < 0)
        _swap(front, back, size);

    if (compare(middle, back) < 0)
        _swap(middle, back, size);

    void* pivot = back;

    for (;; _swap(front, back, size)) {
        while (compare(front += size, pivot) < 0);
        while (compare(pivot, back -= size) < 0);

        if (front >= back) {
            _swap(front, pivot, size);
            return front;
        }
    }
}

void qsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    if (count <= 8)
        return _insertion_sort(data, count, size, compare);

    void* pivot = _partition(data, count, size, compare);
    size_t index = ((char*)pivot - (char*)data) / size;

    qsort(data, index, size, compare);
    qsort(pivot, count - index, size, compare);
}
