/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

static void normal(double x)
{
    for (int exp = -1022; exp < 1024; ++exp) {
        assert(ilogb(x) == exp);
        assert(ilogb(-x) == exp);
        x *= 2;
    }
}

static void subnormal(int64_t i)
{
    for (int exp = -1023; exp > -1075; --exp) {
        double x = reinterpret(double, i >>= 1);
        assert(ilogb(x) == exp);
        assert(ilogb(-x) == exp);
    }
}

int main(void)
{
    assert(ilogb(INFINITY) == INT_MAX);
    assert(ilogb(-INFINITY) == INT_MAX);
    assert(ilogb(0) == FP_ILOGB0);
    assert(ilogb(-0.0) == FP_ILOGB0);

    for (int64_t i = 0x0010000000000000; i < 0x0020000000000000; i += 0x0000000538EF832D) {
        normal(reinterpret(double, i));
        subnormal(i);
    }

    for (uint64_t i = 0x7FF8000000000000; i < 0x8000000000000000; i += 0x000000063416B882) {
        double x = reinterpret(double, i);
        assert(ilogb(x) == FP_ILOGBNAN);
        assert(ilogb(-x) == FP_ILOGBNAN);
    }

}
