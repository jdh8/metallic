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

static void run(float _Complex z)
{
    assert(capprox(cexpf(z), cexp(z)));
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = 0; i < 0x7F800000; i += 0x00098765)
            run(CMPLXF(reinterpret(float, i), reinterpret(float, j)));
}
