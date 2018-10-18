/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <limits.h>
#include <errno.h>

static int _kernel_strtoul_digit(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26u)
        return c - 'a' + 10;

    return INT_MAX;
}

static Unsigned _kernel_strtoul(const char s[restrict static 1], const char* end[restrict static 1], int base, Unsigned max)
{
    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned value = 0;
    _Bool overflow = 0;

    for (int digit = _kernel_strtoul_digit(*s); digit < base; digit = _kernel_strtoul_digit(*s)) {
        Unsigned next = value * base + digit;
        overflow |= threshold < value || next < digit;
        value = next;
        *end = ++s;
    }

    if (overflow || max < value) {
        errno = ERANGE;
        return max;
    }

    return value;
}
