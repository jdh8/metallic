/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "strtol/digit.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>

Signed STRTOL(const char s[restrict static 1], char** restrict end, int base)
{
    Unsigned max = _Generic((Signed)0, long: LONG_MAX, long long: LLONG_MAX);
    Signed sign = 1;
    const char* tail = s;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            max = -(Unsigned)_Generic((Signed)0, long: LONG_MIN, long long: LLONG_MIN);
            sign = -1;
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

    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned magnitude = 0;
    _Bool overflow = 0;

    for (int digit = _strtol_digit(*s); digit < base; digit = _strtol_digit(*s)) {
        Unsigned next = magnitude * base + digit;
        overflow |= threshold < magnitude || next < digit;
        magnitude = next;
        tail = ++s;
    }

    if (end)
        *end = (char*)tail;

    if (overflow || max < magnitude) {
        errno = ERANGE;
        magnitude = max;
    }

    return sign * (Signed)magnitude;
}
