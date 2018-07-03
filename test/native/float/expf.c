/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/expf.c"
#include <assert.h>

int main(void)
{
    assert(_expf(0) == 1);
    assert(_expf(-0.0) == 1);
    assert(_expf(INFINITY) == INFINITY);
    assert(reinterpret(uint32_t, _expf(-INFINITY)) == 0);
    assert(isnan(_expf(NAN)));
    assert(isnan(_expf(-NAN)));

    const float min = log(0x1p-126);
    const float max = log(0x1p+128);
    const float step = 1.337e-5;

    for (float x = min; x < max; x += step)
        verify(approx(_expf(x), exp(x)), x);

    for (uint32_t i = reinterpret(uint32_t, max); i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(_expf(x) == HUGE_VALF, x);
    }

    for (uint32_t i = reinterpret(uint32_t, min); i < 0xFF800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approxf(_expf(x), exp(x)), x);
    }
}
