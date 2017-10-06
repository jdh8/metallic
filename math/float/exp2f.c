/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "expf.h"
#include <math.h>
#include <stdint.h>

float exp2f(float x)
{
    const double ln2 = 0.6931471805599453094;

    if (x < -150)
        return 0;

    if (x > 128)
        return x * HUGE_VALF;

    float n = nearbyintf(x);
    double y = kernel_expf((x - n) * ln2);
    int64_t shifted = *(int64_t*)&y + ((int64_t) n << 52);

    return *(double*)&shifted;
}
