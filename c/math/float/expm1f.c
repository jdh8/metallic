/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static float kernel(float x)
{
    const int b[] = { 30240, 15120, 3360, 420, 30, 1 };

    double a = x;
    double a2 = a * a;
    double a4 = a2 * a2;
    double u = a * (b[5] * a4 + b[3] * a2 + b[1]);
    double v = b[4] * a4 + b[2] * a2 + b[0];

    return 2 * u / (v - u);
}

float expm1f(float x)
{
    const float ln3 = 1.0986123085021973;

    return fabsf(x) <= ln3 ? kernel(x) : expf(x) - 1;
}
