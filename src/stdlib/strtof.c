/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "strtod.h"
#include <ctype.h>
#include <stdint.h>
#include <math.h>

static float _hexfloat(const char s[restrict static 1], const char* end[restrict static 1])
{
    uint32_t x = 0;
    int places = 0;
    int read = 0;
    _Bool pointed = 0;

    *end = s;
    s += 2;

    for (; *s == '0'; ++s)
        *end = s;

    if (*s == '.') {
        pointed = 1;

        for (; *++s == '0'; --places)
            *end = s;
    }

    for (;; *end = ++s) {
        int digit = _xdigit(*s);

        if (digit >= 0) {
            if (++read <= 8)
                x = x << 4 | digit;
        }
        else if (*s == '.' && !pointed) {
            places = read;
            pointed = 1;
        }
        else break;
    }

    if (read < 8)
        x <<= (8 - read) << 2;

    if (!pointed)
        places = read;

    return ldexpf(x, 4 * places - 32 + _exp('p', s, end));
}

static float _scientific(const char s[restrict static 1], const char* end[restrict static 1])
{
    *end = s;
    return 0;
}

static int _match(const char s[static 1], const char t[static 1])
{
    int i;
    for (i = 0; t[i] && (s[i] | 32) == t[i]; ++i);
    return i;
}

static float _nan(const char s[restrict static 1], const char* end[restrict static 1])
{
    *end = s;

    if (*s == '(') {
        while (isalnum(*++s));

        if (*s == ')') {
            float value = nanf(*end + 1);
            *end = s + 1;
            return value;
        }
    }

    return NAN;
}

static float _scan(const char s[restrict static 1], const char* end[restrict static 1])
{
    int match = _match(s, "infinity");

    if (match >= 3) {
        *end = s + (match == 8 ? 8 : 3);
        return INFINITY;
    }

    if (_match(s, "nan") == 3)
        return _nan(s + 3, end);

    if (s[0] == '0' && (s[1] | 32) == 'x')
        return _hexfloat(s, end);

    return _scientific(s, end);
}

float strtof(const char s[restrict static 1], char** restrict end)
{
    const char* tail = s;
    float sign = 1;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    float result = sign * _scan(s, &tail);

    if (end)
        *end = (char*)tail;

    return result;
}
