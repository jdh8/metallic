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
 * This function computes arccosine in a domain of \f$ [0, 1] \f$.
 * The result is guaranteed to be faithfully rounded in \c float,
 * whose relative error is controlled within 9.038906e-9.
 *
 * If \f$ x > 1 \f$, this function returns NaN to indicate complex result.
 * If \f$ x < 0 \f$, the result is inaccurate.
 *
 * \param x - The argument in \f$ [0, \infty] \f$
 * \return  Approximate \f$ \arccos x \f$ as precise as \c float.
 */
static double _kernel(double x)
{
    const double n[] = {
        368.534560209700479,
        326.005089162806504,
         50.1223081718671746
    };

    const double d[] = {
        234.616389739565026,
        239.594332143720441,
         51.3448027474764911,
          1
    };

    double xx = x * x;
    double num = n[0] + n[1] * x + n[2] * xx;
    double den = d[0] + d[1] * x + (d[2] + d[3] * x) * xx;

    return std::sqrt(1 - x) * num / den;
}

extern "C"
float acosf(float x)
{
    double y = _kernel(std::abs(x));

    return std::signbit(x) ? constant::pi - y : y;
}

} // namespace __internal
} // namespace std
