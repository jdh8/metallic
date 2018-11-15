/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include "../../assert.h"
#include <math.h>
#include <stdint.h>

static void run(float x, float y)
{
    _assert(fdimf(x, y) == x - y);
    _assert(fdimf(-y, -x) == x - y);
    _assert(reinterpret(uint32_t, fdimf(y, x)) == 0);
    _assert(reinterpret(uint32_t, fdimf(-x, -y)) == 0);
}

int main(void)
{
    for (uint32_t j = 0; j < 0x7F800000; j += 0x000ABCDE)
        for (uint32_t i = j; i < 0x7F800000; i += 0x00098765)
            run(reinterpret(float, i), reinterpret(float, j));

    _assert(reinterpret(uint32_t, fdimf(INFINITY, INFINITY)) == 0);
    _assert(reinterpret(uint32_t, fdimf(-INFINITY, -INFINITY)) == 0);
}
