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

static double finite_logf(double x)
{
    const double ln2 = 0.6931471805599453094;
    int exponent;

    x = reducef(x, &exponent);

    return 2 * __kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}
