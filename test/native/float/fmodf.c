/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "binary.h"
#include "../../../src/math/float/fmodf.c"
#include <assert.h>
#include <stdint.h>

static void convergent(float x, float y)
{
    float r = fmodf(x, y);

    verify2(identical(r, _fmodf(x, y)), x, y);
    verify2(identical(r, _fmodf(x, -y)), x, y);
    verify2(identical(-r, _fmodf(-x, y)), x, y);
    verify2(identical(-r, _fmodf(-x, -y)), x, y);
}

static void divergent(float x, float y)
{
    verify2(isnan(_fmodf(x, y)), x, y);
}

int main(void)
{
    for (uint32_t j = 6; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            convergent(reinterpret(float, i), reinterpret(float, j));

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        convergent(reinterpret(float, i), INFINITY);

    for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
        quadrants(divergent, reinterpret(float, i), 0);

    for (uint32_t j = 0; j <= 0x7FFFFFFF; j += 0x00100000)
        for (uint32_t i = 0x7F800000; i <= 0x7FFFFFFF; i += 0x00098765)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));
}
