/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <ctype.h>
#include <errno.h>

static unsigned _digit(unsigned c)
{
    if (c - '0' < 10)
        return c - '0';

    c |= 32;

    if (c - 'a' < 26)
        return c - 'a' + 10;

    return -1;
}

static Integer _parseint(const char s[restrict static 1], char** restrict pointer, unsigned base, Unsigned max)
{
    Integer sign = 1;
    const char* end = s;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            max += (Integer)-1 < 0 && ((Integer)-1 & 3) == 3;
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    if (*s == '0') {
        if ((!base || base == 16) && (s[1] | 32) == 'x') {
            base = 16;
            end = s + 1;
            s += 2;
        }
        else if (!base)
            base = 8;
    }
    else if (!base)
        base = 10;

    Unsigned threshold = (Unsigned)-1 / base;
    Unsigned magnitude = 0;
    _Bool overflow = 0;

    for (int digit = _digit(*s); digit < base; digit = _digit(*s)) {
        Unsigned next = magnitude * base + digit;
        overflow |= threshold < magnitude || next < digit;
        magnitude = next;
        end = ++s;
    }

    if (pointer)
        *pointer = end;

    if (overflow || max < magnitude) {
        errno = ERANGE;
        return sign < 0 ? -max : max;
    }

    return sign * (Integer)magnitude;
}
