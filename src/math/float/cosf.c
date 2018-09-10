/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/sincosf.h"

float cosf(float x)
{
    double y;
    unsigned q = __rem_pio2f(x, &y);

    y = q & 1 ? __kernel_sinf(y) : __kernel_cosf(y);

    return (q + 1) & 2 ? -y : y;
}
