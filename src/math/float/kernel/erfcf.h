/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_KERNEL_ERFCF_H
#define METALLIC_KERNEL_ERFCF_H

#include "../prec/expf.h"

static double _kernel_erfcf(double x)
{
    const double c[] = {
       -1.265512220104944281,
        1.000022649889075839,
        0.3741250376812044203,
        0.09638813956533724532,
       -0.1839662244217943646,
        0.2713518068811310958,
       -1.121100483072386768,
        1.473256269115079037,
       -0.8133262681389966881,
        0.1687612120807513779
    };

    double t = 2 / (2 + x);
    double t2 = t * t;
    double t4 = t2 * t2;

    return t * _expf(c[0] + c[1] * t + (c[2] + c[3] * t) * t2
        + (c[4] + c[5] * t + (c[6] + c[7] * t) * t2) * t4
        + (c[8] + c[9] * t) * (t4 * t4)
        - x * x);
}

#endif

/* vim: set ft=c: */
