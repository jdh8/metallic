/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "adigit.h"
#include <ctype.h>
#include <limits.h>
#include <errno.h>

long strtol(const char s[restrict static 1], char** restrict end, int base)
{
    const char* tail = s;
    long extreme = LONG_MAX;
    unsigned long max = LONG_MAX;
    _Bool negative = 0;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            negative = 1;
            extreme = LONG_MIN;
            max = -(unsigned long)extreme;
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

    unsigned long threshold = (unsigned long)-1 / base;
    unsigned long value = 0;
    _Bool overflow = 0;

    for (int digit = _adigit(*s); digit < base; digit = _adigit(*s)) {
        unsigned long next = value * base + digit;
        overflow |= threshold < value || next < digit;
        value = next;
        tail = ++s;
    }

    if (end)
        *end = (char*)tail;

    if (overflow || max < value) {
        errno = ERANGE;
        return extreme;
    }

    long result = value;

    return negative ? -result : result;
}
