/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <float.h>
#include <stdint.h>

static double _coeff(Integer exp)
{
    if (exp >= 1024)
        return DBL_MAX;

    if (exp < -1022)
        return 0x1p-1023;

    return reinterpret(double, (uint64_t)(1023 + exp) << 52);
}

float SCALBNF(float x, Integer exp)
{
    return x * _coeff(exp);
}
