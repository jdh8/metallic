/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef float Scalar;

#define STRTOD strtof
#include "strtod.h"
#include <stdint.h>
#include <float.h>
#include <math.h>

static float _hexfloat(const char s[restrict static 1], const char* end[restrict static 1])
{
    const int capacity = 8;

    uint32_t x = 0;
    int places = 0;
    int read = 0;
    _Bool pointed = 0;

    for (s += 2; *s == '0'; ++s)
        *end = s;

    if (*s == '.') {
        pointed = 1;

        for (; *++s == '0'; --places)
            *end = s;
    }

    for (;; *end = ++s) {
        int digit = _xdigit(*s);

        if (digit >= 0) {
            if (++read <= capacity)
                x = x << 4 | digit;
        }
        else if (*s == '.' && !pointed) {
            places = read;
            pointed = 1;
        }
        else break;
    }

    if (read < capacity)
        x <<= (capacity - read) << 2;

    if (!pointed)
        places = read;

    return ldexpf(x, 4 * (places - capacity) + _exp('p', s, end));
}

static float _scientific(const char s[restrict static 1], const char* end[restrict static 1])
{
    const uint32_t exp10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    const int capacity = FLT_DECIMAL_DIG;

    uint32_t x = 0;
    int places = 0;
    int read = 0;
    _Bool pointed = 0;

    while (*s == '0')
        *end = s++;

    if (*s == '.') {
        pointed = 1;

        for (; *++s == '0'; --places)
            *end = s;
    }

    for (;; *end = ++s) {
        if (*s - '0' < 10u) {
            if (++read <= capacity)
                x = 10 * x + (*s - '0');
        }
        else if (*s == '.' && !pointed) {
            places = read;
            pointed = 1;
        }
        else break;
    }

    if (read < capacity)
        x *= exp10[capacity - read];

    if (!pointed)
        places = read;

    return _scal10n(x, places - capacity + _exp('e', s, end));
}

static float _nan(const char s[restrict static 1], const char* end[restrict static 1])
{
    *end = s;

    if (*s == '(')
        return _strtod_nanf(s + 1, end, ')');

    return NAN;
}
