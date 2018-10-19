/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

void* bsearch(const void* key, const void* begin, size_t count, size_t size, int compare(const void*, const void*))
{
    if (!count)
        return (void*)0;

    size_t half = count >> 1;
    const void* pivot = (const char*)begin + size * half;
    int comparison = compare(key, pivot);

    if (comparison < 0)
        return bsearch(key, begin, half, size, compare);

    if (comparison)
        return bsearch(key, pivot, count - half, size, compare);

    return (void*)pivot;
}
