/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stdlib.h>

int compare(const void* a, const void* b)
{
    unsigned x = *(const unsigned*)a;
    unsigned y = *(const unsigned*)b;

    return (x > y) - (x < y);
}

int main(void)
{
    const int count = 1024;

    unsigned random[count];
    unsigned checksum = 0;

    random[0] = 1337;

    for (int i = 0; i < count - 1; ++i)
        random[i + 1] = random[i] * 48271 % 2147483647;

    for (int i = 0; i < count; ++i)
        checksum += random[i];

    qsort(random, count, sizeof(unsigned), compare);

    for (int i = 0; i < count - 1; ++i)
        _assert(random[i] <= random[i + 1]);

    for (int i = 0; i < count; ++i)
        checksum -= random[i];

    _assert(!checksum);
}
