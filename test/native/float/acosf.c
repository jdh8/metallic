/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/acosf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(acosf(0) * 2 == acosf(-1));
    assert(acosf(0.5) == (float)acos(0.5));

    for (int32_t i = 0; i < 0x3F800000; i += 81) {
        float x = reinterpret(float, i);
        verify(faithful(acosf(x), acos(x)), x);
        verify(faithful(acosf(-x), acos(-x)), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000u; i += 0x100) {
        float x = reinterpret(float, i);
        assert(isnan(acosf(x)));
        assert(isnan(acosf(-x)));
    }
}
