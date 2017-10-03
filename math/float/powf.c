/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "expf/pade.h"
#include "log2f/finite.h"
#include "quietf.h"
#include <math.h>
#include <stdint.h>

static float _exp2fd(double x)
{
    const double ln2 = 0.6931471805599453094;

    if (x < -150)
        return 0;

    if (x > 128)
        return x * HUGE_VALF;

    double n = nearbyint(x);
    double y = expf_pade((x - n) * ln2);
    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    return *(double*)&shifted;
}

static float _unsigned(float x, float y)
{
    if (x == 1)
        return 1;

    if (x == 0)
        return signbit(y) ? HUGE_VALF : 0;

    if (isinf(x))
        return signbit(y) ? 0 : HUGE_VALF;

    if (x > 0)
        return _exp2fd(y * log2f_finite(x));

    return quietf(x);
}

float powf(float x, float y)
{
    uint_least32_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && nearbyintf(y) == y) {
        x = -x;
        sign = -(nearbyintf(y / 2) != y / 2);
    }

    return copysignf(_unsigned(x, y), *(float*)&sign);
}
