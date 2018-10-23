/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../string/copy.h"
#include <stddef.h>
#include <stdint.h>

static void* _assign(void* restrict destination, const void* restrict source, size_t size)
{
    switch (size & -size) {
        top:
            return _copy64(destination, source, size >> 3);
        case 4:
            return _copy32(destination, source, size >> 2);
        case 2:
            return _copy16(destination, source, size >> 1);
        case 1:
            break;
        default:
            goto top;
    }

    return _copy8(destination, source, size);
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

static void _swap64 MEMSWAP(uint64_t);
static void _swap32 MEMSWAP(uint32_t);
static void _swap16 MEMSWAP(uint16_t);
static void _swap8 MEMSWAP(unsigned char);

static void _swap(void* restrict a, void* restrict b, size_t size)
{
    switch (size & -size) {
        top:
            return _swap64(a, b, size >> 3);
        case 4:
            return _swap32(a, b, size >> 2);
        case 2:
            return _swap16(a, b, size >> 1);
        case 1:
            break;
        default:
            goto top;
    }

    return _swap8(a, b, size);
}

static size_t _leaf(void* data, size_t i, size_t count, size_t size, int compare(const void*, const void*))
{ 
    while (2 * i < count) {
        char* base = data;
        i = 2 * i | (compare(base + (2 * i - 1) * size, base + 2 * i * size) < 0);
    }
    return i << (2 * i == count);
}

static void _siftdown(void* data, size_t stem, size_t count, size_t size, int compare(const void*, const void*))
{
    size_t offspring = _leaf(data, stem, count, size, compare);
    char* base = data;
    char buffer[size];

    while (compare(base + size * (offspring - 1), base + size * (stem - 1)) < 0)
        offspring >>= 1;

    _assign(buffer, base + size * (offspring - 1), size);
    _assign(base + size * (offspring - 1), base + size * (stem - 1), size);

    while (stem < offspring) {
        offspring >>= 1;
        _swap(buffer, base + size * (offspring - 1), size);
    }
}

static void _heapsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    for (size_t parent = count >> 1; parent; --parent)
        _siftdown(data, parent, count, size, compare);

    while (count--) {
        _swap(data, (char*)data + count * size, size);
        _siftdown(data, 1, count, size, compare);
    }
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

static void _introsort(void* data, size_t count, size_t size, int ttl, int compare(const void*, const void*))
{
    if (count <= 8)
        return _insertion_sort(data, count, size, compare);

    if (ttl < 0)
        return _heapsort(data, count, size, compare);

    void* pivot = _partition(data, count, size, compare);
    size_t index = ((char*)pivot - (char*)data) / size;

    _introsort(data, index, size, ttl - 1, compare);
    _introsort(pivot, count - index, size, ttl - 1, compare);
}

void qsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    int ttl = __builtin_clz(1) - __builtin_clz(count);

    _introsort(data, count, size, ttl, compare);
}
