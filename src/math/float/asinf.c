/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
/*!
 * \brief Kernel of acosf
 *
 * This function computes arccosine in a domain of \f$ [0.5, 1] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 6.394937e-9.
 *
 * If \f$ x > 1 \f$, this function returns NaN to indicate complex result.
 * If \f$ x < 0.5 \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0.5, \infty] \f$
 * \return  Approximate \f$ \arccos x \f$ as precise as \c float.
 */
static double _acos(double x)
{
    const double c[] = {
        1.57060812392777102,
       -2.12862161326166423e-1,
        8.21583834379219260e-2,
       -3.53431994400216517e-2,
        1.15390721259672731e-2,
       -1.88666274731420876e-3
    };

    double xx = x * x;

    return sqrt(1 - x) * (c[0] + c[1] * x + (c[2] + c[3] * x) * xx + (c[4] + c[5] * x) * (xx * xx));
}

/*!
 * \brief Kernel of asinf
 *
 * This function computes arcsine in a domain of \f$ [-0.5, 0.5] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 4.521903e-9.
 *
 * If \a x is outside of \f$ [-0.5, 0.5] \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [-0.5, 0.5] \f$
 * \return  Approximate \f$ \arcsin x \f$.
 */
static float _asin(double x)
{
    double c[] = {
        9.99999995478096602e-1,
        1.66667922213007301e-1,
        7.49436750021161056e-2,
        4.55576929740685241e-2,
        2.38239365869205885e-2,
        4.26903047180133904e-2
    };

    double x2 = x * x;
    double x4 = x2 * x2;

    return x * (c[0] + c[1] * x2 + (c[2] + c[3] * x2) * x4 + (c[4] + c[5] * x2) * (x4 * x4));
}

float asinf(float x)
{
    const double pi_2 = 1.57079632679489661923;

    float r = fabsf(x);

    return r < 0.5f ? _asin(x) : copysignf(pi_2 - _acos(r), x);
}
