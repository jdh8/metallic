/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../rint.h"
#include <math.h>
/*!
 * \brief Kernel of tanf
 *
 * This computes the limit of \f$ \sqrt x \cot \sqrt x \f$
 * for \a x in \f$ \left[ 0, \frac\pi2 \right] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose absolute error is controlled within 1.366020e-8.
 *
 * If \a x is outside of \f$ \left[ 0, \frac\pi2 \right] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ \left[ 0, \frac\pi2 \right] \f$.
 * \return  Approximate \f$ \sqrt x \cot \sqrt x \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double c[] = {
        1.0000000136602037230,
       -3.3333387464326621934e-1,
       -2.2218602357593262059e-2,
       -2.1257933049346633617e-3,
       -1.9957378945992693876e-4,
       -2.9761078265444352366e-5,
        9.7271696975616268840e-7,
       -7.7386756126714814615e-7
    };

    double xx = x * x;
    double lo = c[0] + c[1] * x + (c[2] + c[3] * x) * xx;
    double hi = c[4] + c[5] * x + (c[6] + c[7] * x) * xx;

    return lo + hi * (xx * xx);
}

float tanf(float x)
{
    const double pi = 3.14159265358979323846;
    const float _1_pi = 0.31830988618379067154;

    double y = x - pi * __rintf(x * _1_pi);

    return y / _kernel(y * y);
}
