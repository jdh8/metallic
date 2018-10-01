/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/powf.c"
#include "binary.h"
#include <assert.h>

static void swapped(float y, float x)
{
    verify2(approx(powf(x, y), pow(x, y), 1), x, y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(swapped, reinterpret(float, j), reinterpret(float, i));

    for (uint32_t i = 0; i < 1 << 10; ++i) {
        float any = reinterpret(float, i << 22);
        verify(powf(1, any) == 1, any);
        verify(powf(any, 0) == 1, any);
        verify(powf(any, -0.0f) == 1, any);
    }

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00100000) {
        float x = reinterpret(float, i);
        mirror(swapped, x, -0.0f);
        mirror(swapped, x, -INFINITY);
        mirror(swapped, INFINITY, -x);
    }

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        for (int y = 0; y < 1024; ++y)
            quadrants(swapped, y, reinterpret(float, i));

    for (uint32_t i = 0x80000001u; i < 0xFF800000u; i += 0x00098765)
        for (float y = -1023.4; y < 1024; ++y)
            assert(isnan(powf(reinterpret(float, i), y)));
}
