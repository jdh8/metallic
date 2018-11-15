/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/double/fmod.h"
#include "binary.h"
#include <assert.h>

static void convergent(double x, double y)
{
    double r = fmod(x, y);

    verify2(identical(r, _fmod(x, y)), x, y);
    verify2(identical(r, _fmod(x, -y)), x, y);
    verify2(identical(-r, _fmod(-x, y)), x, y);
    verify2(identical(-r, _fmod(-x, -y)), x, y);
}

static void divergent(double x, double y)
{
    verify2(isnan(_fmod(x, y)), x, y);
}

int main(void)
{
    const uint64_t inf = 0x7FF0000000000000;
    const uint64_t neg = 0x8000000000000000;
    const uint64_t min = 0x0010000000000000;
    const uint64_t delta = 0x000C79EAA0B3D407;
    const uint64_t model = 0x5B7341E3595E968B;

    for (uint64_t i = 0; i < inf; i += delta) {
        for (int shift = 0; shift < 63; ++shift)
            convergent(reinterpret(double, i), reinterpret(double, model >> shift));
        for (uint64_t mask = 2; mask < 0x8000000000000000; mask <<= 1)
            convergent(reinterpret(double, i), reinterpret(double, (model & -mask) | mask));
    }

    for (uint64_t i = 0; i < inf; i += delta)
        convergent(reinterpret(double, i), INFINITY);

    for (uint64_t i = 0; i < inf; i += delta)
        quadrants(divergent, reinterpret(double, i), 0);

    for (uint64_t i = inf; i < neg; i += delta)
        for (uint64_t j = 0; j < neg; j += min)
            quadrants(divergent, reinterpret(double, i), reinterpret(double, j));
}
