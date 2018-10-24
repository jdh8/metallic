/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
typedef double Scalar;

#define STRTOD strtod
#include "strtod/nan.h"
#include "strtod.h"
#include <stdint.h>
#include <float.h>
#include <math.h>

static double _hexfloat(const char s[restrict static 1], const char* end[restrict static 1])
{
    const int capacity = 16;

    uint64_t x = 0;
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

    return ldexp(x, 4 * (places - capacity) + _exp('p', s, end));
}

static uint64_t _scal10n_u64(uint64_t x, int i)
{
    if (i & 1)
        x *= 10;

    if (i & 2)
        x *= 100;

    if (i & 4)
        x *= 10000;

    if (i & 8)
        x *= 100000000;

    if (i & 16)
        x *= 10000000000000000;

    return x;
}

static double _scientific(const char s[restrict static 1], const char* end[restrict static 1])
{
    const int capacity = DBL_DECIMAL_DIG;

    uint64_t x = 0;
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
        x = _scal10n_u64(x, capacity - read);

    if (!pointed)
        places = read;

    return _scal10n(x, places - capacity + _exp('e', s, end));
}

static double _nan(const char s[restrict static 1], const char* end[restrict static 1])
{
    *end = s;

    if (*s == '(')
        return _strtod_nan(s + 1, end, ')');

    return NAN;
}
