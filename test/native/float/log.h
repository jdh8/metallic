/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "unary.h"

static void test(float f(float), double g(double))
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(faithful(f(x), g(x)), x);
    }

    for (uint32_t i = 0x80000100; i; i += 256) {
        float x = reinterpret(float, i);
        verify(isnan(f(x)), x);
    }

    for (uint32_t i = 0x7FC00000; i <= 0x7FFFFFFF; i += 81) {
        float x = reinterpret(float, i);
        verify(isnan(f(x)), x);
    }
}
