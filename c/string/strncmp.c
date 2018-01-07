/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stddef.h>

int strncmp(const char lhs[static 1], const char rhs[static 1], size_t length)
{
    const unsigned char* a = (const unsigned char*)lhs;
    const unsigned char* b = (const unsigned char*)rhs;

    while (length--) {
        if (*a != *b || *a == 0)
            return *a - *b;
        ++a;
        ++b;
    }
    return 0;
}

