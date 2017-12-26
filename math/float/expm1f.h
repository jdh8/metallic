/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_EXPM1F_H
#define METALLIC_EXPM1F_H

#include "kernel/expm1f.h"
#include "../rint.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

inline double __expm1f(float x)
{
    const float minimum = -103.972077083991796;
    const float maximum = 88.7228391116729996;

    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return x * HUGE_VALF;

    float n = __rintf(x * log2e);
    double y = __kernel_expm1f(x - n * ln2);

    if (n == 0)
        return y;

    y += 1;

    int64_t shifted = __bits(y) + ((int64_t)n << 52);

    return __reinterpret(shifted) - 1;
}
#endif
/* vim: set ft=c: */
