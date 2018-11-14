/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "nan.h"
#include "../../soft/powi/powi.h"
#include <ctype.h>
#include <errno.h>
#include <tgmath.h>

static int _parseexp(int c, const char s[restrict static 1], char* end[restrict static 1])
{
    int sign = 1;
    int magnitude = 0;

    if ((*s | 32) != c)
        return 0;

    switch (*++s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    while (*s == '0')
        *end = (char*)++s;

    for (unsigned digit = *s - '0'; digit < 10; digit = *s - '0') {
        if (magnitude >= 3200) {
            while (*++s - '0' < 10u);
            *end = (char*)s;
            return sign * 32000;
        }
        magnitude = 10 * magnitude + digit;
        *end = (char*)++s;
    }

    return sign * magnitude;
}

static int _min(int a, int b)
{
    return a < b ? a : b;
}

static Scalar _parsehex(const char s[restrict static 1], char* end[restrict static 1])
{
    const int capacity = _Generic((Scalar)0,
        float: 8,
        double: 16,
        long double: 32);

    Bitset x = 0;
    int consumed = 0;
    int position = 0;
    _Bool pointed = 0;

    *end = (char*)(s + 1);

    for (s += 2; *s == '0'; ++s)
        *end = (char*)(s + 1);

    if (*s == '.')
        for (pointed = 1; *++s == '0'; --position)
            *end = (char*)(s + 1);

    for (unsigned digit = _digit(*s); digit < 16 || (*s == '.' && !pointed); digit = _digit(*s)) {
        if (digit < 16) {
            x = x << (4 * (consumed < capacity)) | digit;
            ++consumed;
        }
        else {
            position = consumed;
            pointed = 1;
        }
        *end = (char*)++s;
    }

    if (!pointed)
        position = consumed;

    return ldexp((Scalar)x, 4 * (position - _min(capacity, consumed)) + _parseexp('p', s, end));
}

static uint32_t _powi32(uint32_t x, int i)
{
    uint32_t y = 1;

    for (; i; i >>= 1) {
        if (i & 1)
            y *= x;
        x *= x;
    }
    return y;
}

static uint64_t _fixmul(uint64_t a, uint32_t b, int shift[static 1])
{
    uint64_t low = (a & 0xFFFFFFFF) * b;
    uint64_t high = (a >> 32) * b + (low >> 32);
    int space = __builtin_clz(high >> 32);
    *shift += 32 - space;
    return high << space | (low & 0xFFFFFFFF) >> (32 - space);
}

static double _scaleup(uint64_t significand, int exp)
{
    const uint32_t coeff = _powi32(5, 13);
    int shift = __builtin_ctzll(significand);

    significand >>= shift;
    shift += exp;

    for (; exp >= 13; exp -= 13)
        significand = _fixmul(significand, coeff, &shift);

    significand = _fixmul(significand, _powi32(5, exp), &shift);
    return ldexp(significand, shift);
}

static double _scientific(uint64_t significand, int exp)
{
    if (!significand || exp < -342)
        return 0;

    if (exp > 308)
        return HUGE_VAL;

    if (exp < 0)
        return significand * __builtin_powl(10, exp);

    return _scaleup(significand, exp);
}

static Scalar _parsedec(const char s[restrict static 1], char* end[restrict static 1])
{
    const int capacity = _Generic((Scalar)0,
        float: 32 * 0.3010,
        double: 64 * 0.3010,
        long double: 128 * 0.3010);

    Bitset x = 0;
    int consumed = 0;
    int position = 0;
    _Bool pointed = 0;

    while (*s == '0')
        *end = (char*)++s;

    if (*s == '.')
        for (pointed = 1; *++s == '0'; --position)
            *end = (char*)(s + 1);

    for (unsigned digit = *s - '0'; digit < 10 || (*s == '.' && !pointed); digit = *s - '0') {
        if (digit < 10) {
            x = consumed < capacity ? 10 * x + digit : x | !!digit;
            ++consumed;
        }
        else {
            position = consumed;
            pointed = 1;
        }
        *end = (char*)++s;
    }
    
    if (!pointed)
        position = consumed;

    return _scientific(x, position - _min(capacity, consumed) + _parseexp('e', s, end));
}

static unsigned _match(const char s[static 1], const char t[static 1])
{
    unsigned i = 0;

    while (t[i] && (s[i] | 32) == t[i])
        ++i;

    return i;
}

static Scalar _magnitude(const char s[restrict static 1], char* end[restrict static 1])
{
    unsigned match = _match(s, "infinity");

    if (match >= 3) {
        *end = (char*)(s + (match == 8 ? 8 : 3));
        return INFINITY;
    }

    if (_match(s, "nan") == 3) {
        if (s[3] == '(')
            return _nan(s + 4, end, ')');
        *end = (char*)(s + 3);
        return NAN;
    }

    Scalar finite = (*s == '0' && (s[1] | 32) == 'x' ? _parsehex : _parsedec)(s, end);

    if (finite == INFINITY)
        errno = ERANGE;

    return finite;
}

static Scalar _parsefloat(const char s[restrict static 1], char** restrict pointer)
{
    Scalar sign = 0;
    char* dummy;
    char** end = pointer ? pointer : &dummy;

    *end = (char*)s;
    
    while (isspace(*s))
        ++s;

    switch (*s) {
        case '-':
            sign = -1;
            /* fallthrough */
        case '+':
            ++s;
    }

    return copysign(_magnitude(s, end), sign);
}
