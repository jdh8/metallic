/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "binary.h"
#include "../../../src/math/float/atan2f.c"
#include <assert.h>

static void convergent(float x, float y)
{
    float angle = atan2f(y, x);

    verify2(approx(angle, atan2(y, x), 1), x, y);
    verify2(identical(-angle, atan2f(-y, x)), x, y);
}

static void divergent(float x, float y)
{
    verify2(isnan(atan2f(x, y)), x, y);
    verify2(isnan(atan2f(y, x)), x, y);
}

int main(void)
{
    assert(atan2f(INFINITY, INFINITY) == (float)atan(1));

    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7FC00000; j <= 0x7FFFFFFF; j += 0x000ABCDE)
        for (uint32_t i = 0; j <= 0x7FFFFFFF; j += 0x00100000)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));
}
