/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/reinterpret.h"
#include "../assert.h"
#include "identical.h"
#include <math.h>
#include <stdint.h>

static void normal(double x)
{
    const double mantissa = x * 0x1p1021;

    for (int exp = -1021; exp < 1025; ++exp) {
        int e;
        metallic_assert(frexp(x, &e) == mantissa && e == exp);
        metallic_assert(frexp(-x, &e) == -mantissa && e == exp);
        x *= 2;
    }
}

static void subnormal(int64_t i)
{
    for (int exp = -1022; exp > -1074; --exp) {
        double x = reinterpret(double, i >>= 1);
        double mantissa = (i << (-1022 - exp)) * 0x1p-52;
        int e;
        metallic_assert(frexp(x, &e) == mantissa && e == exp);
        metallic_assert(frexp(-x, &e) == -mantissa && e == exp);
    }
}

int main(void)
{
    int exp;

    metallic_assert(frexp(INFINITY, &exp) == INFINITY);
    metallic_assert(frexp(-INFINITY, &exp) == -INFINITY);

    metallic_assert(identical(frexp(0, &exp), 0) && exp == 0);
    metallic_assert(identical(frexp(-0.0, &exp), -0.0) && exp == 0);

    for (int64_t i = 0x0010000000000000; i < 0x0020000000000000; i += 0x000000066E8F0C50) {
        normal(reinterpret(double, i));
        subnormal(i);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x00000006E954B07A) {
        double x = reinterpret(double, i);
        metallic_assert(isnan(frexp(x, &exp)));
        metallic_assert(isnan(frexp(-x, &exp)));
    }

}
