/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

static void run(double x, double y)
{
    assert(fdim(x, y) == x - y);
    assert(fdim(-y, -x) == x - y);
    assert(reinterpret(uint64_t, fdim(y, x)) == 0);
    assert(reinterpret(uint64_t, fdim(-x, -y)) == 0);
}

int main(void)
{
    for (uint64_t j = 0; j < 0x7FF0000000000000; j += 0x000F357D41AFBD75)
        for (uint64_t i = j; i < 0x7FF0000000000000; i += 0x000EBE116CAB22F4)
            run(reinterpret(double, i), reinterpret(double, j));

    assert(reinterpret(uint64_t, fdim(INFINITY, INFINITY)) == 0);
    assert(reinterpret(uint64_t, fdim(-INFINITY, -INFINITY)) == 0);
}

