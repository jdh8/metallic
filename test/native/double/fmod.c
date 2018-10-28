/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/double/fmod.c"
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
    const uint64_t min = 0x0040000000000000;
    const uint64_t di = 0x003C79EAA0B3D407;
    const uint64_t dj = 0x003E113A788D2DBF;

    for (uint64_t j = 1; j < inf; j += dj)
        for (uint64_t i = 0; i < inf; i += di)
            convergent(reinterpret(double, i), reinterpret(double, j));

    for (uint64_t i = 0; i < inf; i += di)
        convergent(reinterpret(double, i), INFINITY);

    for (uint64_t i = 0; i < inf; i += di)
        quadrants(divergent, reinterpret(double, i), 0);

    for (uint64_t j = 0; j < neg; j += min)
        for (uint64_t i = inf; i < neg; i += di)
            quadrants(divergent, reinterpret(double, i), reinterpret(double, j));
}
