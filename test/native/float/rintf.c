/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/rintf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(_rintf(INFINITY) == INFINITY);
    assert(_rintf(-INFINITY) == -INFINITY);

    for (int32_t i = 0; i < 0x7F800000; i += 77) {
        float x = reinterpret(float, i);
        verify(identical(rintf(x), _rintf(x)), x);
        verify(identical(rintf(-x), _rintf(-x)), x);
    }
    
    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(_rintf(x)));
        assert(isnan(_rintf(-x)));
    }
}
