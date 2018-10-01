/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/float/ctanhf.c"
#include "../../../src/math/float/rem_pio2f.c"
#include "complex.h"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex tanhz = ctanhf(z);

    verify2(capprox(tanhz, ctanh(z), 2, 1), x, y);
    verify2(cidentical(conjf(tanhz), ctanhf(conjf(z))), x, y);
    verify2(cidentical(-tanhz, ctanhf(-z)), x, y);
    verify2(cidentical(-conjf(tanhz), ctanhf(-conjf(z))), x, y);
}

static void divergent(float x, float y)
{
    float _Complex z = ctanhf(CMPLXF(x, y));

    verify2(isnan(crealf(z)), x, y);
    verify2(isnan(cimagf(z)), x, y);
}

static void pole(float x, float y)
{
    verify2(ctanhf(CMPLXF(x, y)) == copysignf(1, x), x, y);
}

static void realnan(float x, float y)
{
    float _Complex z = ctanhf(CMPLXF(x, y));

    verify2(isnan(crealf(z)), x, y);
    verify2(cimagf(z) == 0, x, y);
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x00135769)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            convergent(reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000u; j += 0x00135769)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00123456)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 1; j < 0x7F800000; j += 0x00135769)
        for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 0x00123456)
            quadrants(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000u; j += 0x00135769)
        quadrants(pole, INFINITY, reinterpret(float, j));

    for (uint32_t i = 0x7FC00000; i < 0x80000000u; i += 0x00123456)
        quadrants(realnan, reinterpret(float, i), 0);
}
