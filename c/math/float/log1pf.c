/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "log1pf/quadrature.h"
#include "quietf.h"
#include <math.h>

static double quadrature(double x)
{
    const int degree = 4;

    const double nodes[] = {
        0.0694318442029737124,
        0.3300094782075718676,
        0.6699905217924281324,
        0.9305681557970262876
    };

    const double weights[] = {
        0.1739274225687269287,
        0.3260725774312730713,
        0.3260725774312730713,
        0.1739274225687269287
    };

    double result = -0.0;

    for (int k = 0; k < degree; ++k)
        result += weights[k] * x / (1 + nodes[k] * x);

    return result;
}

static float normal(float x)
{
    const double ln2 = 0.6931471805599452862;
    const int32_t mantissa = 0x007FFFFF;
    const int32_t sqrt2 = 0x3FB504F3;

    float y = x + 1;
    int32_t word = *(int32_t*)&y;
    int exponent = (word >> 23) - 127;

    word = (word & mantissa) | 0x3F800000;

    if (word >= sqrt2) {
        word &= 0xFF7FFFFF;
        ++exponent;
    }

    if (exponent)
        return log1pf_quadrature(*(float*)&word - 1) + exponent * ln2;
    else
        return quadrature(x);
}

float log1pf(float x)
{
    const uint32_t n1 = 0xBF800000;
    const int32_t inf = 0x7F800000;
    const int32_t eps = 0x34000000;

    int32_t i = *(int32_t*)&x;
    uint32_t j = *(uint32_t*)&x;

    if (j == n1)
        return -HUGE_VALF;

    if (j > n1)
        return quietf(x);

    if (i >= inf || j << 1 < eps << 1)
        return x;

    return normal(x);
}
