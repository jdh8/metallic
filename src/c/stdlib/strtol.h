/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/strtoul.h"
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#define _max(T) _Generic((T)0, \
    long: LONG_MAX,            \
    long long: LLONG_MAX,      \
    default: (T)-1             \
)

#define _min(T) _Generic((T)0, \
    long: LONG_MIN,            \
    long long: LLONG_MIN,      \
    default: 0                 \
)

Integer STRTOL(const Character s[restrict static 1], Character** restrict end, int base)
{
    const Character* begin = s;
    Integer extreme = _max(Integer);
    Unsigned threshold = extreme;
    _Bool negative = 0;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            negative = 1;

            if ((Integer)-1 < 0) {
                extreme = _min(Integer);
                threshold = -(Unsigned)extreme;
            }
            /* fallthrough */
        case '+':
            ++s;
    }

    if (*s == '0') {
        if ((!base || base == 16) && _toupper(s[1]) == 'X') {
            s += 2;
            base = 16;
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    struct Conversion conversion = _convert(s, base);

    if (end)
        *end = (Character*)(s == conversion.tail ? begin : conversion.tail);

    if (conversion.overflow || threshold < conversion.value) {
        errno = ERANGE;
        return extreme;
    }

    Integer result = conversion.value;

    return negative ? -result : result;
}
