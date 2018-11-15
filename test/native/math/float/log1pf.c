/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/float/log1pf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(faithful(log1pf(x), log1p(x)), x);
    }

    for (uint32_t i = 0x80000000; i <= 0xBF800000; i += 64) {
        float x = reinterpret(float, i);
        verify(faithful(log1pf(x), log1p(x)), x);
    }

    for (uint32_t i = 0xBF800100; i; i += 256) {
        float x = reinterpret(float, i);
        verify(isnan(log1pf(x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(log1pf(x)));
        assert(isnan(log1pf(-x)));
    }
}