/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/expm1f.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(expm1f(INFINITY) == INFINITY);
    assert(expm1f(-INFINITY) == -1);

    const uint32_t max = reinterpret(uint32_t, (float)log(0x1p+128));

    for (uint32_t i = 0; i < max; i += 77) {
        float x = reinterpret(float, i);
        verify(faithful(expm1f(x), expm1(x)), x);
    }

    for (uint32_t i = 0x80000000u; i < 0xFF800000u; i += 111) {
        float x = reinterpret(float, i);
        verify(faithful(expm1f(x), expm1(x)), x);
    }

    for (uint32_t i = max; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(expm1f(x) == HUGE_VALF, x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(expm1f(x)));
        assert(isnan(expm1f(-x)));
    }
}
