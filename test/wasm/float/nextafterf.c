
/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../assert.h"
#include "identical.h"
#include <math.h>

static _Bool run(float from, float to)
{
    float x = from ? from : copysignf(from, to);
    float y = nextafterf(from, to);
    _Bool near = reinterpret(uint32_t, x) - reinterpret(uint32_t, y) + 1 <= 2;

    if (from == to)
        return identical(to, y);

    if (from < to)
        return from < y && near;

    if (from > to)
        return from > y && near;

    return isnan(y);
}

int main(void)
{
    for (uint32_t j = 0; j < 1 << 12; ++j)
        for (uint32_t i = 0; i < 1 << 12; ++i)
            metallic_assert(run(reinterpret(float, i << 20), reinterpret(float, j << 20)));
}
