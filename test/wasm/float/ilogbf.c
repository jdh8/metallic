/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/reinterpret.h"
#include "../../assert.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

static void normal(float x)
{
    for (int exp = -126; exp < 128; ++exp) {
        metallic_assert(ilogbf(x) == exp);
        metallic_assert(ilogbf(-x) == exp);
        x *= 2;
    }
}

static void subnormal(int32_t i)
{
    for (int exp = -127; exp > -150; --exp) {
        float x = reinterpret(float, i >>= 1);
        metallic_assert(ilogbf(x) == exp);
        metallic_assert(ilogbf(-x) == exp);
    }
}

int main(void)
{
    metallic_assert(ilogbf(INFINITY) == INT_MAX);
    metallic_assert(ilogbf(-INFINITY) == INT_MAX);
    metallic_assert(ilogbf(0) == FP_ILOGB0);
    metallic_assert(ilogbf(-0.0f) == FP_ILOGB0);

    for (int32_t i = 0x00800000; i < 0x01000000; i += 11) {
        normal(reinterpret(float, i));
        subnormal(i);
    }
   
    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        metallic_assert(ilogbf(x) == FP_ILOGBNAN);
        metallic_assert(ilogbf(-x) == FP_ILOGBNAN);
    }

}
