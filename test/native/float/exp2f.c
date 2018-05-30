/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/exp2f.c"
#include <assert.h>

static float _exp2f(float) __attribute__((alias("exp2f")));

int main(void)
{
    assert(_exp2f(0) == 1);
    assert(_exp2f(-0.0) == 1);
    assert(_exp2f(INFINITY) == INFINITY);
    assert(reinterpret(uint32_t, _exp2f(-INFINITY)) == 0);
    assert(isnan(_exp2f(NAN)));

    const float min = -126;
    const float max = 128;
    const float step = 1.337e-5;

    for (float x = min; x < max; x += step)
        verify(approx(_exp2f(x), exp2(x)), x);

    for (uint32_t i = reinterpret(uint32_t, max); i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(_exp2f(x) == HUGE_VALF, x);
    }

    for (uint32_t i = reinterpret(uint32_t, min); i < 0xFF800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approxf(_exp2f(x), exp2(x)), x);
    }
}
