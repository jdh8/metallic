/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/atanhf.c"
#include <assert.h>

int main(void)
{
    assert(_atanhf(1) == INFINITY);
    assert(_atanhf(-1) == -INFINITY);

    for (int32_t i = 0; i < 0x3F800000; i += 81) {
        float x = reinterpret(float, i);
        double y = _atanhf(x);
        verify(approx(y, atanh(x)), x);
        verify(identical(-y, _atanhf(-x)), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000u; i += 0x100) {
        float x = reinterpret(float, i);
        assert(isnan(_atanhf(x)));
        assert(isnan(_atanhf(-x)));
    }
}
