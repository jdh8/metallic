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

#include "kernel/exp2f.h"
#include "../double/shift.h"
#include <math.h>
#include <stdint.h>

static double _exp2f(double x)
{
    if (x < -1022)
        return 0;

    if (x >= 1024)
        return x * HUGE_VAL;

    double n = rint(x);
    double y = 1 + _kernel_exp2f(x - n);

    return _shift(y, n);
}

#endif
