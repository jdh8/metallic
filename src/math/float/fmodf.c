/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalizef.h"
#include "../reinterpret.h"
#include <math.h>

static uint_least32_t _remshift(uint_least32_t a, uint_least32_t b, uint_least32_t exp)
{
    uint_least64_t remainder = ((uint_least64_t)a << (exp & 31)) % b;
    uint_least64_t coeff = ((uint_least64_t)1 << 32) % b;

    for (exp >>= 5; exp; exp >>= 1) {
        if (exp & 1)
            remainder = remainder * coeff % b;
        coeff = coeff * coeff % b;
    }

    return remainder;
}

static int32_t _load(int32_t remainder, int32_t template)
{
    if (remainder == 0)
        return remainder;

    int32_t shift = __builtin_clz(remainder) - 8;
    int32_t exp = (template >> 23) - shift;
    int32_t mantissa = remainder << shift;
    int32_t normalized = exp << 23 | (mantissa & 0x007FFFFF);

    return normalized < 0x00800000 ? mantissa >> -(exp + 1) : normalized;
}

static uint32_t _finite(uint32_t a, uint32_t b)
{
    uint32_t aa = (a & 0x007FFFFF) | 0x00800000;
    uint32_t bb = (b & 0x007FFFFF) | 0x00800000;

    if (b < 0x01000000)
        return a < 0x01000000 ? a % b : _remshift(aa, b, (a >> 23) - 1);

    return _load(_remshift(aa, bb, (a >> 23) - (b >> 23)), b);
}

float fmodf(float numerator, float denominator)
{
    uint32_t a = reinterpret(uint32_t, fabsf(numerator));
    uint32_t b = reinterpret(uint32_t, fabsf(denominator));

    if (a >= 0x7F800000 || b > 0x7F800000 || b == 0)
        return NAN;

    if (a < b)
        return numerator;

    return copysignf(reinterpret(float, _finite(a, b)), numerator);
}
