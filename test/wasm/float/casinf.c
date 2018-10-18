/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../assert.h"
#include "cidentical.h"
#include <complex.h>

static float _Complex swap(float _Complex z)
{
    return CMPLXF(cimagf(z), z);
}

static void run(float x, float y)
{
    metallic_assert(cidentical(casinhf(CMPLXF(x, y)), swap(casinf(CMPLXF(y, x)))));
}

int main(void)
{
    for (uint32_t j = 0; j < 1 << 12; ++j)
        for (uint32_t i = 0; i < 1 << 12; ++i)
            run(reinterpret(float, i << 20), reinterpret(float, j << 20));
}
