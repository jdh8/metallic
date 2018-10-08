/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/asinf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x3F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = asinf(x);
        verify(faithful(y, asin(x)), x);
        verify(identical(-y, asinf(-x)), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000; i += 256) {
        float x = reinterpret(float, i);
        assert(isnan(asinf(x)));
        assert(isnan(asinf(-x)));
    }
}
