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

static float _expf(float) __attribute__((alias("expf")));

int main(void)
{
    const int32_t threshold = 0x42AEAC50; // 126 log(2)

    assert(_expf(INFINITY) == INFINITY);
    assert(reinterpret(uint32_t, _expf(-INFINITY)) == 0);
    assert(isnan(_expf(NAN)));

    for (int32_t i = 0; i < threshold; i += 543) {
        float x = reinterpret(float, i);
        verify(approx(_expf(x), exp(x)), x);
        verify(approx(_expf(-x), exp(-x)), x);
    }

    for (int32_t i = threshold; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(approxf(_expf(x), exp(x)), x);
        verify(approxf(_expf(-x), exp(-x)), x);
    }
}
