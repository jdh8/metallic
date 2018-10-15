/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>

static int _digit(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26u)
        return c - 'a' + 10;

    return INT_MAX;
}

struct Scan
{
    Unsigned value;
    const Character* tail;
    _Bool overflow;
};

static struct Scan _scan(const Character s[static 1], int base)
{
    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned value = 0;
    _Bool overflow = 0;

    for (int digit = _digit(*s); digit < base; digit = _digit(*++s)) {
        Unsigned result = value * base + digit;
        overflow |= threshold < value || result < digit;
        value = result;
    }

    return (struct Scan){ value, s, overflow };
}

Integer STRTOL(const Character s[restrict static 1], Character** restrict end, int base)
{
    const Integer maximum = _Generic((Integer)0,
        long: LONG_MAX,
        long long: LLONG_MAX,
        default: _Generic((Integer)0, intmax_t: INTMAX_MAX, default: -1)
    );

    const Integer minimum = _Generic((Integer)0,
        long: LONG_MIN,
        long long: LLONG_MIN,
        default: _Generic((Integer)0, intmax_t: INTMAX_MIN, default: 0)
    );

    const Character* begin = s;
    Integer extreme = maximum;
    Unsigned threshold = extreme;
    _Bool negative = 0;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            negative = 1;

            if ((Integer)-1 < 0) {
                extreme = minimum;
                threshold = -(Unsigned)extreme;
            }
            /* fallthrough */
        case '+':
            ++s;
    }

    if (*s == '0') {
        if ((!base || base == 16) && (s[1] | 32) == 'x') {
            s += 2;
            base = 16;
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    struct Scan scan = _scan(s, base);

    if (end)
        *end = (Character*)(s == scan.tail ? begin : scan.tail);

    if (scan.overflow || threshold < scan.value) {
        errno = ERANGE;
        return extreme;
    }

    Integer result = scan.value;

    return negative ? -result : result;
}
