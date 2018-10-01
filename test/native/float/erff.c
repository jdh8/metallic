/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/erff.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i < 0x00800000; i += 77) {
        float x = reinterpret(float, i);
        float y = erff(x);
        verify(approx(y, erf(x), 1), x);
        verify(identical(-y, erff(-x)), x);
    }

    for (uint32_t i = 0x00800000; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = erff(x);
        verify(faithful(y, erf(x)), x);
        verify(identical(-y, erff(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i <= 0x7FFFFFFF; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(erff(x)));
        assert(isnan(erff(-x)));
    }
}
