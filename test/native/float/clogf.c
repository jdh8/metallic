/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "common.h"
#include "../../../src/math/float/clogf.c"
#include "../../../src/math/float/rem_pio2f.c"
#include <assert.h>

static void convergent(float x, float y)
{
    float _Complex z = CMPLXF(x, y);
    float _Complex logz = clogf(z);

    verify2(capprox(logz, clog(z), 1, 1), x, y);
    verify2(cidentical(conjf(logz), clogf(conjf(z))), x, y);
}

static void run(void f(float, float), float x, float y)
{
    f(x, y);
    f(-x, y);
}

int main(void)
{
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00100000)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00100000)
            run(convergent, reinterpret(float, i), reinterpret(float, j));
}
