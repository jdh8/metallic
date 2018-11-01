/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static Scalar _powu(Scalar x, unsigned exp)
{
    Scalar y = 1;

    for (; exp; exp >>= 1) {
        if (exp & 1)
            y *= x;
        x *= x;
    }
    return y;
}

Scalar POWI(Scalar x, int exp)
{
    if (exp < 0)
        return _powu(1 / x, -exp);

    return _powu(x, exp);
}
