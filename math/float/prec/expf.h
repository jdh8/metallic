/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_PREC_EXPF_H
#define METALLIC_PREC_EXPF_H

#include "../kernel/expm1f.h"
#include "../../rint.h"
#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

inline double __prec_expf(double x)
{
    const double minimum = -103.972077083991796;
    const double maximum = 88.7228391116729996;

    const double log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return x * HUGE_VALF;

    double n = __rint(x * log2e);
    double y = 1 + __kernel_expm1f(x - n * ln2);
    int64_t shifted = __bits(y) + ((int64_t)n << 52);

    return __reinterpret(shifted);
}

#endif

/* vim: set ft=c: */
