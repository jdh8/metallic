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

#define verify(cond, x) if (!(cond)) {                               \
    fprintf(stderr, "%s:%d: %s: Assertion `"#cond"' failed at %a\n", \
        __FILE__, __LINE__, __func__, (double)(x));                  \
    abort();                                                         \
}

static inline _Bool identical(double x, double y)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, y);
}

static inline _Bool approx(double x, double y, uint64_t tolerance)
{
    uint64_t a = reinterpret(uint64_t, x);
    uint64_t b = reinterpret(uint64_t, y);

    return a - b + tolerance <= 2 * tolerance;
}
