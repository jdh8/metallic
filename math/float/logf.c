/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "logf.h"
#include "quietf.h"
#include <math.h>

float logf(float x)
{
    const int32_t inf = 0x7F800000;

    if (x == 0)
        return -HUGE_VALF;

    int32_t i = *(int32_t*)&x;

    if (i < 0)
        return quietf(x);

    if (i < inf)
        return finite_logf(x);

    return x;
}
