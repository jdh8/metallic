/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _subnormal(uint64_t a, uint64_t b)
{
    //TODO
    return a;
}

static double _normal(uint64_t a, uint64_t b)
{
    //TODO
    return a;
}

static double _finite(uint64_t a, uint64_t b)
{
    const uint64_t threshold = 0x0020000000000000;

    if (b <= threshold)
        return a <= threshold ? reinterpret(double, a % b) : _subnormal(a, b);

    return _normal(a, b);
}

double fmod(double numerator, double denominator)
{
    const uint64_t inf = 0x7FF0000000000000;

    uint64_t a = reinterpret(uint64_t, fabs(numerator));
    uint64_t b = reinterpret(uint64_t, fabs(denominator));

    if (a >= inf || b > inf || b == 0)
        return NAN;

    if (a < b)
        return numerator;

    return copysign(_finite(a, b), numerator);
}
