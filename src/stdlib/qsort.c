/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>
#include <string.h>

static void _swap(void* restrict a, void* restrict b, size_t size)
{
    unsigned char c[size];

    memcpy(c, a, size);
    memcpy(a, b, size);
    memcpy(b, c, size);
}

static size_t _partition(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    char* front = data;
    char* middle = front + size * (count >> 1);
    char* back = front + size * (count - 1);

    if (compare(middle, front) < 0)
        _swap(front, middle);

    if (compare(back, front) < 0)
        _swap(front, back)

    if (compare(middle, back) < 0)
        _swap(middle, back);

    void* pivot = back;

    for (;; _swap(front, back)) {
        while (compare(front += size, pivot) < 0);
        while (compare(back -= size, pivot) > 0);

        if (front >= back) {
            _swap(back, pivot);
        }
    }
}

void qsort(void* data, size_t count, size_t size, int compare(const void*, const void*))
{
    if (count < 2)
        return;

    size_t index = _partition(data, count, size, compare);
    void* pivot = (char*)data + index * size;

    qsort(data, index, size, compare);
    qsort(pivot, count - index, size, compare);
}
