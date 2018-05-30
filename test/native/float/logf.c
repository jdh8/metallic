/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/logf.c"
#include <assert.h>

static float _logf(float) __attribute__((alias("logf")));

int main(void)
{
    assert(reinterpret(uint32_t, _logf(1)) == 0);
    assert(_logf(0) == -INFINITY);
    assert(_logf(-0.0) == -INFINITY);
    assert(_logf(INFINITY) == INFINITY);
    assert(isnan(_logf(-INFINITY)));
    assert(isnan(_logf(NAN)));

    for (int32_t i = 0; i < 0x7F800000; i += 77) {
        float x = reinterpret(float, i);
        verify(approx(_logf(x), log(x)), x);
    }

    for (uint32_t i = 1; i < 0x80000000; i += 12345) {
        float x = reinterpret(float, i);
        verify(isnan(_logf(-x)), x);
    }
}
