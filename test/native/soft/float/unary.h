/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include "assertf.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define verify(cond, a) ASSERTF(cond,         \
    "at 0x%.16"PRIx64"%.16"PRIx64"\n",        \
    (uint64_t)((unsigned __int128)(a) >> 64), \
    (uint64_t)(unsigned __int128)(a))

static inline _Bool approx(unsigned __int128 a, unsigned __int128 b, unsigned __int128 tolerance)
{
    return a - b + tolerance <= 2 * tolerance;
}
