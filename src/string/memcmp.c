/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

int memcmp(const void* lhs, const void* rhs, size_t length)
{
    const unsigned char* x = lhs;
    const unsigned char* y = rhs;

    while (length--) {
        unsigned char a = *x++;
        unsigned char b = *y++;

        if (a != b)
            return a - b;
    }

    return 0;
}

