/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
/*!
 * \brief Kernel of acosf
 *
 * This function computes arccosine in a domain of \f$ [0, 1] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 2.179963e-8.
 *
 * If \f$ x > 1 \f$, this function returns NaN to indicate complex result.
 * If \f$ x < 0 \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0, \infty] \f$
 * \return  Approximate \f$ \arccos x \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double c[] = {
        1.5707963049952700155,
       -2.1459880383414681170e-1,
        8.8979049893610385888e-2,
       -5.0174715211875860817e-2,
        3.0893053200289071461e-2,
       -1.7089810818777579223e-2,
        6.6712932693206083568e-3,
       -1.2628309202213843948e-3
    };

    double xx = x * x;

    return sqrt(1 - x) * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx
        + (c[4] + c[5] * x + (c[6] + c[7] * x) * xx) * (xx * xx));
}

static float _acosf(float x)
{
    const double pi = 3.14159265358979323846;
    double y = _kernel(fabsf(x));

    return x > 0 ? y : pi - y;
}

#ifdef _METALLIC
float acosf(float x) { return _acosf(x); }
#endif
