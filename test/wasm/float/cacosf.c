/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../assert.h"
#include "cidentical.h"
#include <complex.h>

static float _Complex rotate(float _Complex z)
{
    return CMPLXF(-cimagf(z), z);
}

static void run(float _Complex z)
{
    metallic_assert(cidentical(cacoshf(z), rotate(cacosf(z))));
    metallic_assert(cidentical(conjf(cacosf(z)), cacosf(conjf(z))));
}

int main(void)
{
    for (uint32_t j = 0; j < 0x80000000; j += 0x00100000)
        for (uint32_t i = 0; i < 0x80000000; i += 0x00100000)
            run(CMPLXF(reinterpret(float, i), reinterpret(float, j)));
}
