/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/expf.h"
#include <math.h>
/*!
 * \brief Kernel of erff
 *
 * This function computes error function in \f$ [-0.5, 0.5] \f$
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 1.467829e-9.
 *
 * \param x - The argument in \f$ [-0.5, 0.5] \f$
 * \return  Approximate \f$ \operatorname{erf} x \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double c[] = {
        1.128379165627683563,
       -3.761260933840203448e-1,
        1.128283804130954422e-1,
       -2.675815571034888124e-2,
        4.719928166232619326e-3
    };

    double x2 = x * x;
    double x4 = x2 * x2;

    return x * (c[0] + c[1] * x2 + (c[2] + c[3] * x2) * x4 + c[4] * (x4 * x4));
}

static double _complement(double x)
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

float erff(float x)
{
    float r = fabsf(x);

    return r > 0.4769362762044699f ? copysignf(1 - _complement(r), x) : _kernel(x);
}
