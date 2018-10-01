/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/csqrtf.c"
#include "complex.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex sqrtz = csqrtf(z);

    verify2(capprox(sqrtz, csqrt(z), 1, 1), x, y);
    verify2(cidentical(conjf(sqrtz), csqrtf(conjf(z))), x, y);
}

static void divergent(float x, float y)
{
    float _Complex z = csqrtf(CMPLXF(x, y));

    verify2(isnan(crealf(z)), x, y);
    verify2(isnan(cimagf(z)), x, y);
}

static void positive(float y, float unused)
{
    float _Complex z = csqrtf(CMPLXF(INFINITY, y));

    verify(crealf(z) == INFINITY, y);
    verify(isnan(cimagf(z)), y);
}

static void negative(float y, float unused)
{
    float _Complex z = csqrtf(CMPLXF(-INFINITY, y));

    verify(isnan(crealf(z)), y);
    verify(isinf(cimagf(z)), y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            mirror(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 0x00123456)
        mirror(convergent, reinterpret(float, i), INFINITY);

    for (uint32_t j = 0x7FC00000; j < 0x80000000u; j += 0x00135769) {
        for (uint32_t i = 0; i < 0x80000000u; i += 0x00123456) {
            float x = reinterpret(float, i);
            float y = reinterpret(float, j);
            mirror(divergent, x, y);
            mirror(divergent, x, -y);
            mirror(divergent, y, x);
            mirror(divergent, y, -x);
        }
    }

    for (uint32_t j = 0x7FC00000; j < 0x80000000u; j += 0x00135769)
        mirror(positive, reinterpret(float, j), 0);

    for (uint32_t j = 0x7FC00000; j < 0x80000000u; j += 0x00135769)
        mirror(negative, reinterpret(float, j), 0);
}
