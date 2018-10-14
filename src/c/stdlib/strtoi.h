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
#include <errno.h>

static int _toupper(int c)
{
    return c & ~0x20u;
}

static int _atoi(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c = _toupper(c);

    if (c - 'A' < 26u)
        return c - 'A' + 10;

    return INT_MAX;
}

struct Conversion
{
    Unsigned value;
    const Character* tail;
    _Bool overflow;
};

static struct Conversion _convert(const Character s[static 1], int base)
{
    Unsigned max = (Unsigned)-1 / base;
    Unsigned value = 0;
    _Bool overflow = 0;

    for (int digit = _atoi(*s); digit < base; digit = _atoi(*++s)) {
        Unsigned result = value * base + digit;
        overflow |= max < value || result < digit;
        value = result;
    }

    return (struct Conversion){ value, s, overflow };
}

Integer CONVERT(const Character s[restrict static 1], Character** restrict end, int base)
{
    const Character* begin = s;
    Integer extreme = _max;
    Unsigned threshold = extreme;
    _Bool negative = 0;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            negative = 1;

            if ((Integer)-1 < 0) {
                extreme = _min;
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
