/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

static size_t _match(const char s[static 1], const char t[static 1])
{
    size_t i;
    for (i = 0; t[i] && (s[i] | 32) == t[i]; ++i);
    return i;
}

struct Scan
{
    float value;
    const char* tail;
};

static struct Scan _nan(const char s[static 1])
{
    float value = NAN;

    if (*s == '(') {
        const char* t;
        for (t = s + 1; isalnum(*t); ++t);

        if (*t == ')') {
            value = nanf(s + 1);
            s = t + 1;
        }
    }

    return (struct Scan){ value, s };
}

static int _xdigit(int c)
{
    if (c - '0' < 10u)
        return c - '0';

    c |= 32;

    if (c - 'a' < 6u)
        return c - 'a' + 10;

    return -1;
}

static struct Scan _hexadecimal(const char s[static 1])
{
    struct Scan fallback = { 0, s - 2 };

    uint32_t x = 0;
    int shift = 0;
    _Bool valid = 0;
    _Bool pointed = 0;

    for (; *s == '0'; ++s)
        valid = 1;

    if (*s == '.') {
        pointed = 1;

        for (; *++s == '0'; --shift)
            valid = 1;
    }

    for (int i = 0; ;) {
        int digit = _xdigit(*s);

        if (digit >= 0) {
            if (i < 8)
                x = x << 4 | digit;
            valid = 1;
            ++i;
        }
        else if (!pointed) {
            shift = i;
            if (*s == '.') pointed = 1;
            else break;
        }
        else break;
    }

    return fallback;
}

static struct Scan _decimal(const char s[static 1])
{
    float value = 0;

    return (struct Scan){ value, s };
}

static struct Scan _scan(const char s[static 1])
{
    size_t match = _match(s, "infinity");

    if (match >= 3)
        return (struct Scan){ INFINITY, s + 3 + 5 * (match == 8) };

    if (_match(s, "nan") == 3)
        return _nan(s + 3);

    if (s[0] == '0' && (s[1] | 32) == 'x')
        return _hexadecimal(s + 2);

    return _decimal(s);
}

float strtof(const char s[restrict static 1], char** restrict end)
{
    const char* begin = s;
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

    struct Scan scan = _scan(s);

    if (end)
        *end = (char*)(s == scan.tail ? begin : scan.tail);

    return sign * scan.value;
}
