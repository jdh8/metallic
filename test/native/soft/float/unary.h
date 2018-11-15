/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

_Noreturn void abort(void);

#define verify(cond, a) if (!(cond)) {                                                       \
    unsigned __int128 _a = a;                                                                \
    fprintf(stderr, "%s:%d: %s: Assertion `"#cond"' failed at 0x%016"PRIx64"%016"PRIx64"\n", \
        __FILE__, __LINE__, __func__, (uint64_t)(_a >> 64), (uint64_t)_a);                   \
    abort();                                                                                 \
}

static inline _Bool approx(unsigned __int128 a, unsigned __int128 b, unsigned __int128 tolerance)
{
    return a - b + tolerance <= 2 * tolerance;
}
