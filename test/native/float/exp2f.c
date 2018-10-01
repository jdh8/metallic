/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"
#include "../../../src/math/float/exp2f.c"
#include <assert.h>

int main(void)
{
    assert(exp2f(0) == 1);
    assert(exp2f(-0.0) == 1);
    assert(exp2f(INFINITY) == INFINITY);
    assert(reinterpret(uint32_t, exp2f(-INFINITY)) == 0);

    const float min = -126;
    const float max = 128;
    const float step = 1.337e-5;

    for (float x = min; x < max; x += step)
        verify(faithful(exp2f(x), exp2(x)), x);

    for (uint32_t i = reinterpret(uint32_t, max); i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(exp2f(x) == HUGE_VALF, x);
    }

    for (uint32_t i = reinterpret(uint32_t, min); i < 0xFF800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approx(exp2f(x), exp2(x), 1), x);
    }
    
    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(exp2f(x)));
        assert(isnan(exp2f(-x)));
    }
}
