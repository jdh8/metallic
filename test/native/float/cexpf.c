/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/cexpf.c"
#include "../../../src/math/float/rem_pio2f.c"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex expz = cexpf(z);

    verify2(capprox(expz, cexp(z), 1, 1), x, y);
    verify2(cidentical(conjf(expz), cexpf(conjf(z))), x, y);
}

static void divergent(float x, float y)
{
    float _Complex upper = cexpf(CMPLXF(x, y));
    float _Complex lower = cexpf(CMPLXF(x, -y));

    verify2(isnan(crealf(upper)), x, y);
    verify2(isnan(cimagf(upper)), x, y);
    verify2(isnan(crealf(lower)), x, -y);
    verify2(isnan(cimagf(lower)), x, -y);
}

static void run(void f(float, float), float x, float y)
{
    f(x, y);
    f(-x, y);
}

static void pole(float x)
{
    verify(isinf(crealf(cexpf(CMPLXF(INFINITY, x)))), x);
    verify(isnan(cimagf(cexpf(CMPLXF(INFINITY, x)))), x);
    verify(cexpf(CMPLXF(-INFINITY, x)) == 0, x);
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x00135769)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            run(convergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000u; j += 0x00135769)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00123456)
            run(divergent, reinterpret(float, i), reinterpret(float, j));

    for (uint32_t j = 0x7F800000; j < 0x80000000u; j += 0x00135769) {
        float x = reinterpret(float, j);
        pole(x);
        pole(-x);
    }
}
