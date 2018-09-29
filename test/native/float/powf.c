/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/powf.c"
#include <assert.h>

static void real(float x, float y)
{
    assert(approx(powf(x, y), pow(x, y), 1));
    assert(approx(powf(x, -y), pow(x, -y), 1));
}

static void integral(float x, float y)
{
    assert(approx(powf(x, y), pow(x, y), 1));
    assert(approx(powf(-x, y), pow(-x, y), 1));
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            real(reinterpret(float, i), reinterpret(float, j));

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765) {
        float x = reinterpret(float, i);
        real(-0.0f, x);
        real(INFINITY, x);
        real(-INFINITY, x);
        real(x, INFINITY);
        real(-x, INFINITY);
        real(x, -INFINITY);
        real(-x, -INFINITY);
    }

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        for (int y = -1024; y < 1024; ++y)
            integral(reinterpret(float, i), y);

    for (uint32_t i = 0x80000001; i < 0xFF800000; i += 0x00098765)
        for (float y = -1023.4; y < 1024; ++y)
            assert(isnan(powf(reinterpret(float, i), y)));

    for (uint32_t i = 0; i < 1 << 10; ++i) {
        float any = reinterpret(float, i << 22);
        verify(powf(1, any) == 1, any);
        verify(powf(any, 0) == 1, any);
        verify(powf(any, -0.0f) == 1, any);
    }
}
