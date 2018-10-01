/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../../src/math/reinterpret.h"
#include <stdint.h>
#include <float.h>
#include <inttypes.h>
#include <stdio.h>

_Noreturn void abort(void);

#define verify(cond, x) if (!(cond)) {                                    \
    fprintf(stderr, "Assertion `"#cond"' failed at %g [0x%08"PRIx32"]\n", \
        (float)(x), reinterpret(uint32_t, (float)(x)));                   \
    abort();                                                              \
}

static inline _Bool identical(float x, float y)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, y);
}

static inline _Bool faithful(double x, double y)
{
    const uint64_t mask = 0x1FFFFFFF;
    uint64_t a = reinterpret(uint64_t, x);
    uint64_t b = reinterpret(uint64_t, y);

    return a - b + mask <= 2 * mask;
}

static inline _Bool approx(float x, float y, unsigned tolerance)
{
    uint32_t a = reinterpret(uint32_t, x);
    uint32_t b = reinterpret(uint32_t, y);

    return a - b + tolerance <= 2 * tolerance;
}
