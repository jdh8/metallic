/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

static void _swap(unsigned char* restrict a, unsigned char* restrict b, size_t size)
{
    #if defined(__clang__) && defined(__OPTIMIZE__) && !defined(__OPTIMIZE_SIZE__)
    #pragma clang loop vectorize(enable)
    #endif
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = a[i];
        a[i] = b[i];
        b[i] = c;
    }
}

static void* _partition(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    unsigned char* front = data;
    unsigned char* middle = front + size * (count >> 1);
    unsigned char* back = front + size * (count - 1);

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
    if (count < 2)
        return;

    void* pivot = _partition(data, count, size, compare);
    size_t index = ((char*)pivot - (char*)data) / size;

    qsort(data, index, size, compare);
    qsort(pivot, count - index, size, compare);
}
