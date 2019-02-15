/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
/*!\file
 * \brief Complementary error function
 */
#include "expf.h"
#include <math.h>
/*!
 * \brief Kernel of complementary error function
 *
 * This function computes complementary error function of a positive argument.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 1.387597e-8.
 *
 * \param x - The argument in \f$ [0, \infty] \f$
 * \return  Approximate \f$ \operatorname{erfc} x \f$ as precise as \c float.
 */
static double _right(double x)
{
    const double c[] = {
       -1.2655414002954096370,
        1.0009261252013356806,
        0.36249638433463102684,
        0.17824481408677383361,
       -0.53449623821044308004,
        1.2320178646153913321,
       -2.8400914385649108826,
        3.4701646981231999064,
       -2.2636614184225674494,
        0.76671522289756803240,
       -0.10677459988959471943
    };

    double t = 2 / (2 + x);
    double t2 = t * t;
    double t4 = t2 * t2;

    return t * _expf(c[0] + c[1] * t + (c[2] + c[3] * t) * t2
        + (c[4] + c[5] * t + (c[6] + c[7] * t) * t2) * t4
        + (c[8] + c[9] * t + c[10] * t2) * (t4 * t4)
        - x * x);
}

float erfcf(float x)
{
    double t = _right(fabsf(x));

    return signbit(x) ? 2 - t : t;
}
