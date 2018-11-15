/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/float/coshf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(coshf(INFINITY) == INFINITY);
    assert(coshf(-INFINITY) == INFINITY);

    const float max = log(0x1p129);
    const float step = 1.337e-5;

    for (float x = 0; x < max; x += step) {
        float y = coshf(x);
        verify(faithful(y, cosh(x)), x);
        verify(identical(y, coshf(-x)), x);
    }

    for (uint32_t i = reinterpret(uint32_t, max) + 1; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(coshf(x) == HUGE_VALF, x);
        verify(coshf(-x) == HUGE_VALF, x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(coshf(x)));
        assert(isnan(coshf(-x)));
    }
}
