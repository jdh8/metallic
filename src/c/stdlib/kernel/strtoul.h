/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <limits.h>

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
