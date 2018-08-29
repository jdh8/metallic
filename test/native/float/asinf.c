/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/asinf.c"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i < 0x3F800000; i += 81) {
        float x = reinterpret(float, i);
        float y = _asinf(x);
        assert(_asinf(-x) == -y);
        verify(approx(asin(x), y), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000u; i += 0x100) {
        float x = reinterpret(float, i);
        assert(isnan(_asinf(x)));
        assert(isnan(_asinf(-x)));
    }
}
