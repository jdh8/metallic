/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!\file
 * \brief Error function
 */
#include "expf.h"
#include <math.h>
/*!
 * \brief Kernel of the error function
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
       -1.265537997442867325,
        1.000753302741609634,
        0.3654581268239641747,
        0.1524812799668063738,
       -0.4023184988741130035,
        0.8037376242882822036,
       -1.938889435057737967,
        2.241083589001155299,
       -1.215499682756722948,
        0.2587662485942665068
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
