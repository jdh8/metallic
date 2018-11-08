/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../getc.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>

static int _digit(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26u)
        return c - 'a' + 10;

    return INT_MAX;
}

static Integer _scaninteger(FILE stream[static 1], int base)
{
    const Unsigned min = _Generic((Integer)0,
        signed char: SCHAR_MIN,
        short: SHRT_MIN,
        int: INT_MIN,
        long: LONG_MIN,
        long long: LLONG_MIN,
        default: 0);

    Unsigned max = _Generic((Integer)0,
        signed char: SCHAR_MAX,
        short: SHRT_MAX,
        int: INT_MAX,
        long: LONG_MAX,
        long long: LLONG_MAX,
        default: -1);

    Integer sign = 1;
    int cache;

    while (isspace(cache = getc(stream)));

    switch (cache) {
        case '-':
            max = min ? -min : -1;
            sign = -1;
            /* fallthrough */
        case '+':
            cache = getc(stream);
    }

    if (cache == '0') {
        if ((!base || base == 16) && ((cache = getc(stream)) | 32) == 'x') {
            base = 16;
            cache = getc(stream);
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned magnitude = 0;
    _Bool overflow = 0;

    for (int digit = _digit(cache); digit < base; digit = _digit(getc(stream))) {
        Unsigned next = magnitude * base + digit;
        overflow |= threshold < magnitude || next < digit;
        magnitude = next;
    }

    if (overflow || max < magnitude) {
        errno = ERANGE;
        return sign < 0 ? -max : max;
    }

    return sign * (Integer)magnitude;
}
