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

static int32_t _mantissa(int32_t a)
{
    return (a & 0x007FFFFF) | 0x00800000;
}

static int32_t _load(int32_t mantissa, int32_t template)
{
    if (mantissa == 0)
        return mantissa;

    int32_t shift = __builtin_clz(mantissa) - 8;
    int32_t bias = -shift << 23 | (mantissa << shift & 0x007FFFFF);
    int32_t normalized = bias + (template & 0x7F800000);

    if (normalized < 0x00800000)
        return _mantissa(normalized) >> -((normalized >> 23) + 1);

    return normalized;
}

static int32_t _finite(int32_t a, int32_t b)
{
    unsigned exp = (a >> 23) - (b >> 23);
    uint_least32_t divisor = _mantissa(b);
    uint_least64_t remainder = ((uint_least64_t)_mantissa(a) << (exp & 31)) % divisor;
    uint_least64_t coeff = 0x100000000 % divisor;

    for (exp >>= 5; exp; exp >>= 1) {
        if (exp & 1)
            remainder = remainder * coeff % divisor;
        coeff = coeff * coeff % divisor;
    }

    return _load(remainder, b);
}

float fmodf(float numerator, float denominator)
{
    int32_t a = reinterpret(int32_t, numerator);
    int32_t b = reinterpret(int32_t, denominator) & 0x7FFFFFFF;
    int32_t s = a & 0x80000000;

    a &= 0x7FFFFFFF;

    if (a >= 0x7F800000 || b > 0x7F800000 || b == 0)
        return reinterpret(float, a | 0x7FC00000);

    if (a < b)
        return numerator;

    return reinterpret(float, _finite(__normalizef(a), __normalizef(b)) | s);
}
