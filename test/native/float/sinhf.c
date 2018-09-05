/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/sinhf.c"
#include <assert.h>

int main(void)
{
    assert(_sinhf(INFINITY) == INFINITY);
    assert(_sinhf(-INFINITY) == -INFINITY);
    assert(isnan(_sinhf(NAN)));
    assert(isnan(_sinhf(-NAN)));

    const float max = reinterpret(uint32_t, (float)log(0x1p129));

    for (uint32_t i = 0; i < max; i += 77) {
        float x = reinterpret(float, i);
        float y = _sinhf(x);
        verify(approx(y, sinh(x)), x);
        assert(reinterpret(uint32_t, -y) == reinterpret(uint32_t, _sinhf(-x)));
    }

    for (uint32_t i = max + 1; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(_sinhf(x) == HUGE_VALF, x);
        verify(_sinhf(-x) == -HUGE_VALF, x);
    }
}
