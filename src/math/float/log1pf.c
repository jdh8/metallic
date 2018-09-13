/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/log1pf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

float log1pf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i == 0xBF800000u)
        return -HUGE_VALF;

    if (i > 0xBF800000u)
        return NAN;

    if (i < 0x7F800000)
        return _log1pf(x);

    return x;
}
