/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanhf.h"
#include "reducef.h"

static double finite_log2f(double x)
{
    const double log2_e2 = 2.8853900817779268147;

    int exponent;

    x = reducef(x, &exponent);

    return log2_e2 * __kernel_atanhf((x - 1) / (x + 1)) + exponent;
}
