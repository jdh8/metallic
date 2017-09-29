/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static float log1pf_quadrature(float x)
{
    const int degree = 4;

    const float nodes[] = {
        0.0694318442029737124,
        0.3300094782075718676,
        0.6699905217924281324,
        0.9305681557970262876
    };

    const float weights[] = {
        0.1739274225687269287,
        0.3260725774312730713,
        0.3260725774312730713,
        0.1739274225687269287
    };

    float result = -0.0;

    for (int k = 0; k < degree; ++k)
        result += weights[k] * x / (1 + nodes[k] * x);

    return result;
}
