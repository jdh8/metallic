/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <ctype.h>

Integer ATOI(const char s[static 1])
{
    Integer magnitude = 0;
    Integer sign = 1;
    
    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    for (unsigned digit = *s - '0'; digit < 10; digit = *s - '0') {
        magnitude = 10 * magnitude + digit;
        ++s;
    }

    return sign * magnitude;
}
