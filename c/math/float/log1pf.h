/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static float __log1pf(float x)
{
    const int degree = 5;

    const double nodes[] = {
        0.0469100770306680036,
        0.2307653449471584545,
        0.5000000000000000000,
        0.7692346550528415455,
        0.9530899229693319964
    };

    const double weights[] = {
        0.1184634425280945438,
        0.2393143352496832340,
        0.2844444444444444444,
        0.2393143352496832340,
        0.1184634425280945438
    };

    double result = 0;

    for (int k = 0; k < degree; ++k)
        result += weights[k] * x / (1 + nodes[k] * x);

    return result;
}
