/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"
#include "../../../src/math/float/log1pf.c"
#include <assert.h>

int main(void)
{
    assert(log1pf(-1) == -INFINITY);
    assert(log1pf(INFINITY) == INFINITY);
    assert(isnan(log1pf(-INFINITY)));

    for (int32_t i = 0; i < 0x7F800000; i += 77) {
        float x = reinterpret(float, i);
        verify(faithful(log1pf(x), log1p(x)), x);
    }

    for (uint32_t i = 0xBF800001; i >> 31; i += 12345) {
        float x = reinterpret(float, i);
        verify(isnan(log1pf(x)), x);
    }
    
    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(log1pf(x)));
        assert(isnan(log1pf(-x)));
    }
}
