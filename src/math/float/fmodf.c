/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static uint32_t _significand(uint32_t i)
{
    return (i & 0x007FFFFF) | 0x00800000;
}

static uint32_t _remshift(uint32_t a, uint32_t b, uint32_t exp)
{
    uint_fast64_t remainder = ((uint_fast64_t)a << (exp & 31)) % b;

    for (uint32_t shift = exp >> 5; shift; --shift)
        remainder = (remainder << 32) % b;

    return remainder;
}

static int32_t _load(int32_t remainder, int32_t template)
{
    if (remainder == 0)
        return remainder;

    int32_t shift = __builtin_clz(remainder) - 8;
    int32_t exp = (template >> 23) - shift;
    int32_t significand = remainder << shift;
    int32_t normalized = exp << 23 | (significand & 0x007FFFFF);

    return normalized < 0x00800000 ? significand >> (1 - exp) : normalized;
}

static uint32_t _finite(uint32_t a, uint32_t b)
{
    if (b <= 0x01000000)
        return a <= 0x01000000 ? a % b : _remshift(_significand(a), b, (a >> 23) - 1);

    return _load(_remshift(_significand(a), _significand(b), (a >> 23) - (b >> 23)), b);
}

static float _fmodf(float numerator, float denominator)
{
    uint32_t a = reinterpret(uint32_t, fabsf(numerator));
    uint32_t b = reinterpret(uint32_t, fabsf(denominator));

    if (a >= 0x7F800000 || b > 0x7F800000 || b == 0)
        return NAN;

    if (a < b)
        return numerator;

    return copysignf(reinterpret(float, _finite(a, b)), numerator);
}

#ifdef _METALLIC
float fmodf(float numerator, float denominator)
{
    return _fmodf(numerator, denominator);
}
#endif
