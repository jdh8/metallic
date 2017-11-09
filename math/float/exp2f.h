/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>
/*!
 * \brief Kernel of exp2f
 *
 * This computes exponential functon in a domain of \f$ [-0.5,
 * 0.5] \f$.  The result is guaranteed to be faithfully rounded in
 * \c float, whose relative error is controlled within 5.136187e-9.
 *
 * If \a x is outside of \f$ [-0.5, 0.5] \f$,
 * the result is inaccurate.
 *
 * \param x - The argument in \f$ [-0.5, 0.5] \f$
 * \return  Approximate \f$ 2^x - 1 \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double c[] = {
        2.885390086914114281,
        1.155241599178795343e-1,
       -9.211145068253212300e-4
    };

    double xx = x * x;
    double xcothxln2_2 = c[0] + c[1] * xx + c[2] * (xx * xx);

    return 2 * x / (xcothxln2_2 - x);
}

static Scalar generic_exp2f(Scalar x)
{
    const double ln2 = 0.6931471805599453094;

    if (x < -150)
        return 0;

    if (x > 128)
        return x * HUGE_VALF;

    Scalar n = _TGMATH_REAL(x, nearbyint)(x);
    double y = 1 + _kernel(x - n);
    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    return *(double*)&shifted;
}
