/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include <stdint.h>
#include <stdlib.h>

int compare(const void* a, const void* b)
{
    return (const int*)a - (const int*)b;
}

int main(void)
{
    enum { count = 1024 };

    int random[count];

    random[0] = 1337;

    for (int i = 0; i < count - 1; ++i)
        random[i + 1] = random[i] * 48271u % 2147483647;

    qsort(random, count, sizeof(int), compare);

}
