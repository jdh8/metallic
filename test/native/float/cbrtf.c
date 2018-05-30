/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/cbrtf.c"
#include <assert.h>

static float _cbrtf(float) __attribute__((alias("cbrtf")));

int main(void)
{
    assert(_cbrtf(INFINITY) == INFINITY);
    assert(_cbrtf(-INFINITY) == -INFINITY);

    for (uint32_t i = 0; i < 0x7F800000; i += 543) {
        float x = reinterpret(float, i);
        float y = _cbrtf(x);
        verify(approx(y, cbrt(x)), x);
        verify(_cbrtf(-x) == -y, x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 543) {
        float x = reinterpret(float, i);
        verify(isnan(_cbrtf(x)), x);
        verify(isnan(_cbrtf(-x)), x);
    }
}
