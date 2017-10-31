// This file is part of Metallic, a runtime library for WebAssembly.
//
// Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <internal/constant>
#include <cmath>

namespace std {
namespace __internal {
/*!
 * \brief Kernel of acosf
 *
 * This function computes arccosine in a domain of \f$ [0.5, 1] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 3.669929e-9.
 *
 * If \f$ x > 1 \f$, this function returns NaN to indicate complex result.
 * If \f$ x < 0.5 \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0.5, \infty] \f$
 * \return  Approximate \f$ \arccos x \f$ as precise as \c float.
 */
static double _acos22(double x)
{
    const double n[] = {
        19.3409975360328623,
        11.6478870037997229,
         0.700950333090353189
    };

    const double d[] = {
        12.3131674101366886,
         9.09492966503933472,
         1
    };

    double xx = x * x;
    double num = n[0] + n[1] * x + n[2] * xx;
    double den = d[0] + d[1] * x + d[2] * xx;

    return std::sqrt(1 - x) * num / den;
}

/*!
 * \brief Kernel of asinf
 *
 * This function computes arcsine in a domain of \f$ [0, 0.5] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 2.595032e-9.
 *
 * If \a x is outside of \f$ [0, 0.5] \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0, 0.5] \f$
 * \return  Approximate \f$ \arcsin x \f$.
 */
static double _asin22(double x)
{
    double n[] = {
        80.3271869581482272,
        72.2722811622844352,
        11.0979939649070095
    };

    double d[] = {
        5.60202426879400611,
       -5.54746483251966753,
        1
    };

    double x2 = x * x;
    double x4 = x2 * x2;
    double num = n[0] + n[1] * x2 + n[2] * x4;
    double den = d[0] + d[1] * x2 + d[2] * x4;

    return x * num / den;
}

/*!
 * \brief Kernel of asinf
 *
 * This function computes arcsine in a domain of \f$ [0, 1] \f$.
 *
 * If \f$ x > 1 \f$, this function returns NaN to indicate complex result.
 * If \f$ x < 0 \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0, \infty] \f$
 * \return  Approximate \f$ \arcsin x \f$.
 */
static float _kernel(float x)
{
    return x < 0.5f ? _asin22(x) : constant::pi_2 - _acos22(x);
}

extern "C"
float asinf(float x)
{
    return std::copysign(_kernel(std::abs(x)), x);
}

} // namespace __internal
} // namespace std
