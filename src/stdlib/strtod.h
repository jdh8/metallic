/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
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
