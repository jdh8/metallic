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
#include <math.h>
#include <stdlib.h>

static int _xdigit(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c |= 32;

    if (c - 'a' < 6u)
        return c - 'a' + 10;

    return -1;
}

static int _unsigned(const char s[restrict static 1], const char* end[restrict static 1])
{
    int value = 0;

    for (; *s - '0' < 10u; *end = ++s) {
        if (value >= 3000) {
            for (++s; *s - '0' < 10u; *end = ++s);
            return 30000;
        }
        value = *s - '0' + 10 * value;
    }

    return value;
}

static int _exp(int c, const char s[restrict static 1], const char* end[restrict static 1])
{
    int sign = 1;

    if ((*s | 32) != c)
        return 0;

    switch (*++s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    for (; *s == '0'; ++s)
        *end = s;

    return sign * _unsigned(s, end);
}

static Scalar _scal10n(Scalar x, int e)
{
    Scalar factor = e < 0 ? (Scalar)1/10 : 10;

    for (e = abs(e); e; e >>= 1) {
        if (e & 1)
            x *= factor;
        factor *= factor;
    }

    return x;
}

static int _match(const char s[static 1], const char t[static 1])
{
    int i;
    for (i = 0; t[i] && (s[i] | 32) == t[i]; ++i);
    return i;
}

static Scalar _nan(const char[restrict static 1], const char*[restrict static 1]);
static Scalar _hexfloat(const char[restrict static 1], const char*[restrict static 1]);
static Scalar _scientific(const char[restrict static 1], const char*[restrict static 1]);

static Scalar _scan(const char s[restrict static 1], const char* end[restrict static 1])
{
    int match = _match(s, "infinity");

    if (match >= 3) {
        *end = s + (match == 8 ? 8 : 3);
        return INFINITY;
    }

    if (_match(s, "nan") == 3)
        return _nan(s + 3, end);

    Scalar finite = (s[0] == '0' && (s[1] | 32) == 'x' ? _hexfloat : _scientific)(s, end);

#ifndef SUPPRESS_ERANGE
    if (finite == INFINITY)
        errno = ERANGE;
#endif

    return finite;
}

Scalar STRTOD(const char s[restrict static 1], char** restrict end)
{
    const char* tail = s;
    Scalar sign = 1;

    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    Scalar result = sign * _scan(s, &tail);

    if (end)
        *end = (char*)tail;

    return result;
}
