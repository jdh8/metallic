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

static int _toupper(int c)
{
    return c & ~0x20u;
}

static int _extract(int c)
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
    unsigned long value;
    const char* tail;
    _Bool overflow;
};

static struct Conversion _convert(const char s[restrict static 1], int base)
{
    unsigned long max = (unsigned long)-1 / base;
    unsigned long value = 0;
    _Bool overflow = 0;

    for (int digit = _extract(*s); digit < base; digit = _extract(*++s)) {
        unsigned long result = value * base + digit;
        overflow |= max < value || result < digit;
        value = result;
    }

    return (struct Conversion){ value, s, overflow };
}

long strtol(const char s[restrict static 1], char** restrict end, int base)
{
    long overflow = LONG_MAX;
    long result = 0;
    int sign = 1;
    const char* tail = s;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            overflow = LONG_MIN;
            sign = -1;
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

    for (int digit = _extract(*s); digit < base; digit = _extract(*++s)) {
        tail = s;

        if (__builtin_smull_overflow(result, base, &result) || __builtin_saddl_overflow(result, digit * sign, &result)) {
            while (_extract(*++tail) < base);
            result = overflow;
            break;
        }
    }

    if (end)
        *end = (char*)tail;

    return result;
}
