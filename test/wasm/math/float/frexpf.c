/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include "../../assert.h"
#include "identical.h"
#include <math.h>
#include <stdint.h>

static void normal(float x)
{
    const float mantissa = x * 0x1p125f;

    for (int exp = -125; exp < 129; ++exp) {
        int e;
        _assert(frexpf(x, &e) == mantissa && e == exp);
        _assert(frexpf(-x, &e) == -mantissa && e == exp);
        x *= 2;
    }
}

static void subnormal(int32_t i)
{
    for (int exp = -126; exp > -149; --exp) {
        float x = reinterpret(float, i >>= 1);
        float mantissa = (i << (-126 - exp)) * 0x1p-23f;
        int e;
        _assert(frexpf(x, &e) == mantissa && e == exp);
        _assert(frexpf(-x, &e) == -mantissa && e == exp);
    }
}

int main(void)
{
    int exp;

    _assert(frexpf(INFINITY, &exp) == INFINITY);
    _assert(frexpf(-INFINITY, &exp) == -INFINITY);

    _assert(identical(frexpf(0, &exp), 0) && exp == 0);
    _assert(identical(frexpf(-0.0f, &exp), -0.0f) && exp == 0);

    for (int32_t i = 0x00800000; i < 0x01000000; i += 11) {
        normal(reinterpret(float, i));
        subnormal(i);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        _assert(isnan(frexpf(x, &exp)));
        _assert(isnan(frexpf(-x, &exp)));
    }

}
