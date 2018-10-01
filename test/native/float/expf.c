/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"
#include "../../../src/math/float/expf.c"
#include <assert.h>

int main(void)
{
    assert(expf(0) == 1);
    assert(expf(-0.0) == 1);
    assert(expf(INFINITY) == INFINITY);
    assert(reinterpret(uint32_t, expf(-INFINITY)) == 0);

    const float min = log(0x1p-126);
    const float max = log(0x1p+128);
    const float step = 1.337e-5;

    for (float x = min; x < max; x += step)
        verify(faithful(expf(x), exp(x)), x);

    for (uint32_t i = reinterpret(uint32_t, max); i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(expf(x) == HUGE_VALF, x);
    }

    for (uint32_t i = reinterpret(uint32_t, min); i < 0xFF800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approx(expf(x), exp(x), 1), x);
    }
    
    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(expf(x)));
        assert(isnan(expf(-x)));
    }
}
