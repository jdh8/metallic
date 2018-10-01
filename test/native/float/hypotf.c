/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "binary.h"
#include "../../../src/math/float/hypotf.c"
#include <assert.h>

static void convergent(float x, float y)
{
    float r = hypotf(x, y);

    verify2(approx(r, hypot(x, y), 1), x, y);
    verify2(identical(r, hypotf(y, x)), x, y);
    verify2(identical(r, hypotf(-x, y)), x, y);
    verify2(identical(r, hypotf(-y, x)), x, y);
    verify2(identical(r, hypotf(-x, -y)), x, y);
    verify2(identical(r, hypotf(-y, -x)), x, y);
    verify2(identical(r, hypotf(x, -y)), x, y);
    verify2(identical(r, hypotf(y, -x)), x, y);
}

static void divergent(float x, float y)
{
    verify2(isnan(hypotf(x, y)), x, y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = j; i <= 0x7F800000; i += 0x00100000)
            convergent(reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7FC00000; j <= 0x7FFFFFFF; j += 0x000ABCDE)
        convergent(INFINITY, reinterpret(float, j));

    for (uint32_t j = 0x7FC00000; j <= 0x7FFFFFFF; j += 0x000ABCDE)
        for (uint32_t i = 0x7FC00000; j <= 0x7FFFFFFF; j += 0x00098765)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));
}
