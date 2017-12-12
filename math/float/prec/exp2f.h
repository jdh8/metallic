/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_PREC_EXP2F_H
#define METALLIC_PREC_EXP2F_H

#include "../../reinterpret.h"
#include "../kernel/exp2f.h"
#include <math.h>
#include <stdint.h>

inline double __prec_exp2f(double x)
{
    if (x < -150)
        return 0;

    if (x > 128)
        return x * HUGE_VALF;

    double n = nearbyint(x);
    double y = 1 + __kernel_exp2f(x - n);
    int64_t shifted = __bits(y) + ((int64_t)n << 52);

    return __reinterpret(shifted);
}

#endif